/**
 * @file class_reader.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include "byte_code.hpp"
#include "class.hpp"
#include "constant_pool.hpp"

namespace jasm
{

    void Class::read_class(std::istream &is)
    {
        u4 magic = read_big_endian<u4>(is);
        assert(magic == Magic);
        minor_version_ = read_big_endian<u2>(is);
        major_version_ = read_big_endian<u2>(is);
        u2 constant_pool_count = read_big_endian<u2>(is);

        // The value of the constant_pool_count item is equal to the number of entries in the
        // constant_pool table plus one
        assert(constant_pool_count > 0);

        for (u2 i = 1; i < constant_pool_count; ++i) {
            u1 tag = read_big_endian<u1>(is);
            read_constant(is, tag);
        }

        access_flags_ = read_big_endian<u2>(is);
        this_class_ = read_big_endian<u2>(is);
        super_class_ = read_big_endian<u2>(is);
        u2 interfaces_count = read_big_endian<u2>(is);

        for (u2 i = 0; i < interfaces_count; ++i) {
            u2 interface_index = read_big_endian<u2>(is);
            assert(dynamic_cast<ClassConstant *>(constant_pool_[interface_index]) != nullptr);
            interfaces_.push_back(interface_index);
        }

        u2 fields_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < fields_count; ++i) {
            u2 field_access_flags = read_big_endian<u2>(is);
            u2 name_index = read_big_endian<u2>(is);
            u2 descriptor_index = read_big_endian<u2>(is);

            Field field = Field(field_access_flags, name_index, descriptor_index);

            u2 attributes_count = read_big_endian<u2>(is);
            for (u2 j = 0; j < attributes_count; ++j) {
                read_attribute(is, &field);
            }

            add_field(std::move(field));
        }

        u2 methods_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < methods_count; ++i) {
            u2 method_access_flags = read_big_endian<u2>(is);
            u2 name_index = read_big_endian<u2>(is);
            u2 descriptor_index = read_big_endian<u2>(is);

            Method method = Method(method_access_flags, name_index, descriptor_index);

            u2 attributes_count = read_big_endian<u2>(is);
            for (u2 j = 0; j < attributes_count; ++j) {
                read_attribute(is, &method);
            }

            add_method(std::move(method));
        }

        u2 attributes_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < attributes_count; ++i) {
            read_attribute(is, this);
        }
    }

    void Class::read_constant(std::istream &is, u1 tag)
    {
        switch (tag) {
            case ConstantPool::CONSTANT_UTF_8: {
                u2 length = read_big_endian<u2>(is);
                u1 bytes[length];
                is.read((char *) bytes, length);
                constant_pool_.make_constant<Utf8Constant>(length, bytes);
                break;
            }
            case ConstantPool::CONSTANT_INTEGER: {
                u4 bytes = read_big_endian<u4>(is);
                constant_pool_.make_constant<IntegerConstant>(bytes);
                break;
            }
            case ConstantPool::CONSTANT_FLOAT: {
                u4 bytes = read_big_endian<u4>(is);
                constant_pool_.make_constant<FloatConstant>(bytes);
                break;
            }
            case ConstantPool::CONSTANT_LONG: {
                u4 high_bytes = read_big_endian<u4>(is);
                u4 low_bytes = read_big_endian<u4>(is);
                constant_pool_.make_constant<LongConstant>(high_bytes, low_bytes);
                break;
            }
            case ConstantPool::CONSTANT_DOUBLE: {
                u4 high_bytes = read_big_endian<u4>(is);
                u4 low_bytes = read_big_endian<u4>(is);
                constant_pool_.make_constant<DoubleConstant>(high_bytes, low_bytes);
                break;
            }
            case ConstantPool::CONSTANT_CLASS: {
                u2 name_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<ClassConstant>(name_index);
                break;
            }
            case ConstantPool::CONSTANT_STRING: {
                u2 string_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<StringConstant>(string_index);
                break;
            }
            case ConstantPool::CONSTANT_FIELD_REF: {
                u2 class_index = read_big_endian<u2>(is);
                u2 name_and_type_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<FieldRefConstant>(class_index, name_and_type_index);
                break;
            }
            case ConstantPool::CONSTANT_METHOD_REF: {
                u2 class_index = read_big_endian<u2>(is);
                u2 name_and_type_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<MethodRefConstant>(class_index, name_and_type_index);
                break;
            }
            case ConstantPool::CONSTANT_INTERFACE_METHOD_REF: {
                u2 class_index = read_big_endian<u2>(is);
                u2 name_and_type_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<InterfaceMethodRefConstant>(class_index,
                                                                         name_and_type_index);
                break;
            }
            case ConstantPool::CONSTANT_NAME_AND_TYPE: {
                u2 name_index = read_big_endian<u2>(is);
                u2 descriptor_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<NameAndTypeConstant>(name_index, descriptor_index);
                break;
            }
            case ConstantPool::CONSTANT_METHOD_HANDLE: {
                u2 reference_kind = read_big_endian<u2>(is);
                u2 reference_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<MethodHandleConstant>(reference_kind, reference_index);
                break;
            }
            case ConstantPool::CONSTANT_METHOD_TYPE: {
                u2 descriptor_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<MethodTypeConstant>(descriptor_index);
                break;
            }
            case ConstantPool::CONSTANT_INVOKE_DYNAMIC: {
                u2 bootstrap_method_attr_index = read_big_endian<u2>(is);
                u2 name_and_type_index = read_big_endian<u2>(is);
                constant_pool_.make_constant<InvokeDynamicConstant>(bootstrap_method_attr_index,
                                                                    name_and_type_index);
                break;
            }
            default:
                assert(false);
        }
    }

    void Class::read_attribute(std::istream &is, Attributable *attr)
    {
        u2 attribute_name_index = read_big_endian<u2>(is);
        u4 attribute_length = read_big_endian<u4>(is);

        auto *attribute_name_const = dynamic_cast<Utf8Constant *>(
                constant_pool_[attribute_name_index]
        );
        assert(attribute_name_const != nullptr);

        std::string attribute_name = attribute_name_const->value();

        if (attribute_name == "SourceFile") {
            u2 source_file_index = read_big_endian<u2>(is);
            attr->make_attribute<SourceFileAttribute>(attribute_name_index, source_file_index);
        } else if (attribute_name == "Code") {
            u2 max_stack = read_big_endian<u2>(is);
            u2 max_locals = read_big_endian<u2>(is);
            CodeAttribute code(attribute_name_index, max_stack, max_locals);

            u4 code_length = read_big_endian<u4>(is);
            for (u4 i = 0; i < code_length;) {
                i += read_instruction(is, &code);
            }

            u2 exception_table_length = read_big_endian<u2>(is);
            for (u2 i = 0; i < exception_table_length; ++i) {
                u2 start_pc = read_big_endian<u2>(is);
                u2 end_pc = read_big_endian<u2>(is);
                u2 handler_pc = read_big_endian<u2>(is);
                u2 catch_type = read_big_endian<u2>(is);
                code.make_exception_table_entry(start_pc, end_pc, handler_pc, catch_type);
            }

            u2 attributes_count = read_big_endian<u2>(is);
            for (u2 i = 0; i < attributes_count; ++i) {
                read_attribute(is, &code);
            }

            attr->add_attribute(std::move(code));
        } else {
            // skip unknown
            std::cerr << "Warning: attribute " << attribute_name << "is not implemented." << std::endl;
            for (u4 i = 0; i < attribute_length; ++i) {
                read_big_endian<u1>(is);
            }
        }
    }

    u4 Class::read_instruction(std::istream &is, CodeAttribute *code)
    {
        u1 opcode = read_big_endian<u1>(is);
        switch (opcode) {
            case 0x12: {
                u1 index = read_big_endian<u1>(is);
                code->make_instruction<LoadConst>(index);
                return 2;
            }
            case 0x2a:
                code->make_instruction<RefLoad0>();
                return 1;
            case 0xb1:
                code->make_instruction<Return>();
                return 1;
            case 0xb2: {
                u1 index_byte_1 = read_big_endian<u1>(is);
                u1 index_byte_2 = read_big_endian<u1>(is);
                code->make_instruction<GetStatic>(index_byte_1, index_byte_2);
                return 3;
            }
            case 0xb6: {
                u1 index_byte_1 = read_big_endian<u1>(is);
                u1 index_byte_2 = read_big_endian<u1>(is);
                code->make_instruction<InvokeVirtual>(index_byte_1, index_byte_2);
                return 3;
            }
            case 0xb7: {
                u1 index_byte_1 = read_big_endian<u1>(is);
                u1 index_byte_2 = read_big_endian<u1>(is);
                code->make_instruction<InvokeSpecial>(index_byte_1, index_byte_2);
                return 3;
            }
            default:
                // skip unimplemented instruction
                std::cerr << "Warning: instruction " << std::hex << std::setw(2) << std::setfill('0')
                          << opcode << " (" << InstructionMnemonics[opcode] << ") is not implemented."
                          << std::endl;
                return 1;
        }
    }

    void Class::jasm(std::ostream &os) const
    {
        // dump the constant pool
        size_t i = 1;
        for (auto &c : constant_pool_) {
            std::cout << '#' << std::setw(4) << std::left << i++ << " = ";
            c->jasm(std::cout);
        }
        std::cout << std::endl;

        // dump attributes
        for (auto &attr : attributes_) {
            attr->jasm(std::cout, &constant_pool_);
        }
        std::cout << std::endl;

        // dump fields
        for (auto &field : fields_) {
            field.jasm(std::cout, &constant_pool_);
            std::cout << std::endl;
        }

        // dump methods
        for (auto &method : methods_) {
            method.jasm(std::cout, &constant_pool_);
            std::cout << std::endl;
        }
    }

    void Class::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u4>(os, Magic);
        write_big_endian<u2>(os, minor_version_);
        write_big_endian<u2>(os, major_version_);

        write_big_endian<u2>(os, constant_pool_.count() + 1);
        for (auto &constant : constant_pool_)
            constant->emit_bytecode(os);

        write_big_endian<u2>(os, access_flags_);
        write_big_endian<u2>(os, this_class_);
        write_big_endian<u2>(os, super_class_);

        write_big_endian<u2>(os, interfaces_.size());
        for (auto interface : interfaces_)
            write_big_endian<u2>(os, interface);

        write_big_endian<u2>(os, fields_.size());
        for (auto &field : fields_)
            field.emit_bytecode(os);

        write_big_endian<u2>(os, methods_.size());
        for (auto &method : methods_)
            method.emit_bytecode(os);

        write_big_endian<u2>(os, attributes_.size());
        for (auto &attr : attributes_)
            attr->emit_bytecode(os);
    }

}