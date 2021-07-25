/**
 * @file class_reader.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include "class.hpp"
#include "byte_code.hpp"
#include "constant_pool.hpp"

namespace jasm {

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
            // 8 byte constants take up two entries in the constant pool
            if (tag == ConstantPool::CONSTANT_DOUBLE || tag == ConstantPool::CONSTANT_LONG)
                ++i;
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
            constant_pool_.make_constant<EmptyConstant>();
            break;
        }
        case ConstantPool::CONSTANT_DOUBLE: {
            u4 high_bytes = read_big_endian<u4>(is);
            u4 low_bytes = read_big_endian<u4>(is);
            constant_pool_.make_constant<DoubleConstant>(high_bytes, low_bytes);
            constant_pool_.make_constant<EmptyConstant>();
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
            constant_pool_.make_constant<InterfaceMethodRefConstant>(class_index, name_and_type_index);
            break;
        }
        case ConstantPool::CONSTANT_NAME_AND_TYPE: {
            u2 name_index = read_big_endian<u2>(is);
            u2 descriptor_index = read_big_endian<u2>(is);
            constant_pool_.make_constant<NameAndTypeConstant>(name_index, descriptor_index);
            break;
        }
        case ConstantPool::CONSTANT_METHOD_HANDLE: {
            u2 reference_kind = read_big_endian<u1>(is);
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
            constant_pool_.make_constant<InvokeDynamicConstant>(bootstrap_method_attr_index, name_and_type_index);
            break;
        }
        default:
            std::cerr << "Error: invalid constant tag " << (int) tag << "." << std::endl;
            assert(false);
        }
    }

    void Class::read_attribute(std::istream &is, Attributable *attr)
    {
        u2 attribute_name_index = read_big_endian<u2>(is);
        u4 attribute_length = read_big_endian<u4>(is);

        auto *attribute_name_const = dynamic_cast<Utf8Constant *>(constant_pool_[attribute_name_index]);
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
                assert(i <= code_length);
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
            std::cerr << "Warning: attribute " << attribute_name << " is not implemented." << std::endl;
            for (u4 i = 0; i < attribute_length; ++i) {
                read_big_endian<u1>(is);
            }
        }
    }

#define CASE_SIMPLE_INST(OPCODE)                                                                                       \
    case OPCODE:                                                                                                       \
        code->make_instruction<SimpleInstruction<OPCODE>>(&is);                                                        \
        return 1 + InstructionInfo[opcode][0];

    u4 Class::read_instruction(std::istream &is, CodeAttribute *code)
    {
        u1 opcode = read_big_endian<u1>(is);
        switch (opcode) {
            CASE_SIMPLE_INST(0x00)
            CASE_SIMPLE_INST(0x01)
            CASE_SIMPLE_INST(0x02)
            CASE_SIMPLE_INST(0x03)
            CASE_SIMPLE_INST(0x04)
            CASE_SIMPLE_INST(0x05)
            CASE_SIMPLE_INST(0x06)
            CASE_SIMPLE_INST(0x07)
            CASE_SIMPLE_INST(0x08)
            CASE_SIMPLE_INST(0x09)
            CASE_SIMPLE_INST(0x0a)
            CASE_SIMPLE_INST(0x0b)
            CASE_SIMPLE_INST(0x0c)
            CASE_SIMPLE_INST(0x0d)
            CASE_SIMPLE_INST(0x0e)
            CASE_SIMPLE_INST(0x0f)
            CASE_SIMPLE_INST(0x10)
            CASE_SIMPLE_INST(0x11)
            CASE_SIMPLE_INST(0x12)
            CASE_SIMPLE_INST(0x13)
            CASE_SIMPLE_INST(0x14)
            CASE_SIMPLE_INST(0x15)
            CASE_SIMPLE_INST(0x16)
            CASE_SIMPLE_INST(0x17)
            CASE_SIMPLE_INST(0x18)
            CASE_SIMPLE_INST(0x19)
            CASE_SIMPLE_INST(0x1a)
            CASE_SIMPLE_INST(0x1b)
            CASE_SIMPLE_INST(0x1c)
            CASE_SIMPLE_INST(0x1d)
            CASE_SIMPLE_INST(0x1e)
            CASE_SIMPLE_INST(0x1f)
            CASE_SIMPLE_INST(0x20)
            CASE_SIMPLE_INST(0x21)
            CASE_SIMPLE_INST(0x22)
            CASE_SIMPLE_INST(0x23)
            CASE_SIMPLE_INST(0x24)
            CASE_SIMPLE_INST(0x25)
            CASE_SIMPLE_INST(0x26)
            CASE_SIMPLE_INST(0x27)
            CASE_SIMPLE_INST(0x28)
            CASE_SIMPLE_INST(0x29)
            CASE_SIMPLE_INST(0x2a)
            CASE_SIMPLE_INST(0x2b)
            CASE_SIMPLE_INST(0x2c)
            CASE_SIMPLE_INST(0x2d)
            CASE_SIMPLE_INST(0x2e)
            CASE_SIMPLE_INST(0x2f)
            CASE_SIMPLE_INST(0x30)
            CASE_SIMPLE_INST(0x31)
            CASE_SIMPLE_INST(0x32)
            CASE_SIMPLE_INST(0x33)
            CASE_SIMPLE_INST(0x34)
            CASE_SIMPLE_INST(0x35)
            CASE_SIMPLE_INST(0x36)
            CASE_SIMPLE_INST(0x37)
            CASE_SIMPLE_INST(0x38)
            CASE_SIMPLE_INST(0x39)
            CASE_SIMPLE_INST(0x3a)
            CASE_SIMPLE_INST(0x3b)
            CASE_SIMPLE_INST(0x3c)
            CASE_SIMPLE_INST(0x3d)
            CASE_SIMPLE_INST(0x3e)
            CASE_SIMPLE_INST(0x3f)
            CASE_SIMPLE_INST(0x40)
            CASE_SIMPLE_INST(0x41)
            CASE_SIMPLE_INST(0x42)
            CASE_SIMPLE_INST(0x43)
            CASE_SIMPLE_INST(0x44)
            CASE_SIMPLE_INST(0x45)
            CASE_SIMPLE_INST(0x46)
            CASE_SIMPLE_INST(0x47)
            CASE_SIMPLE_INST(0x48)
            CASE_SIMPLE_INST(0x49)
            CASE_SIMPLE_INST(0x4a)
            CASE_SIMPLE_INST(0x4b)
            CASE_SIMPLE_INST(0x4c)
            CASE_SIMPLE_INST(0x4d)
            CASE_SIMPLE_INST(0x4e)
            CASE_SIMPLE_INST(0x4f)
            CASE_SIMPLE_INST(0x50)
            CASE_SIMPLE_INST(0x51)
            CASE_SIMPLE_INST(0x52)
            CASE_SIMPLE_INST(0x53)
            CASE_SIMPLE_INST(0x54)
            CASE_SIMPLE_INST(0x55)
            CASE_SIMPLE_INST(0x56)
            CASE_SIMPLE_INST(0x57)
            CASE_SIMPLE_INST(0x58)
            CASE_SIMPLE_INST(0x59)
            CASE_SIMPLE_INST(0x5a)
            CASE_SIMPLE_INST(0x5b)
            CASE_SIMPLE_INST(0x5c)
            CASE_SIMPLE_INST(0x5d)
            CASE_SIMPLE_INST(0x5e)
            CASE_SIMPLE_INST(0x5f)
            CASE_SIMPLE_INST(0x60)
            CASE_SIMPLE_INST(0x61)
            CASE_SIMPLE_INST(0x62)
            CASE_SIMPLE_INST(0x63)
            CASE_SIMPLE_INST(0x64)
            CASE_SIMPLE_INST(0x65)
            CASE_SIMPLE_INST(0x66)
            CASE_SIMPLE_INST(0x67)
            CASE_SIMPLE_INST(0x68)
            CASE_SIMPLE_INST(0x69)
            CASE_SIMPLE_INST(0x6a)
            CASE_SIMPLE_INST(0x6b)
            CASE_SIMPLE_INST(0x6c)
            CASE_SIMPLE_INST(0x6d)
            CASE_SIMPLE_INST(0x6e)
            CASE_SIMPLE_INST(0x6f)
            CASE_SIMPLE_INST(0x70)
            CASE_SIMPLE_INST(0x71)
            CASE_SIMPLE_INST(0x72)
            CASE_SIMPLE_INST(0x73)
            CASE_SIMPLE_INST(0x74)
            CASE_SIMPLE_INST(0x75)
            CASE_SIMPLE_INST(0x76)
            CASE_SIMPLE_INST(0x77)
            CASE_SIMPLE_INST(0x78)
            CASE_SIMPLE_INST(0x79)
            CASE_SIMPLE_INST(0x7a)
            CASE_SIMPLE_INST(0x7b)
            CASE_SIMPLE_INST(0x7c)
            CASE_SIMPLE_INST(0x7d)
            CASE_SIMPLE_INST(0x7e)
            CASE_SIMPLE_INST(0x7f)
            CASE_SIMPLE_INST(0x80)
            CASE_SIMPLE_INST(0x81)
            CASE_SIMPLE_INST(0x82)
            CASE_SIMPLE_INST(0x83)
            CASE_SIMPLE_INST(0x84)
            CASE_SIMPLE_INST(0x85)
            CASE_SIMPLE_INST(0x86)
            CASE_SIMPLE_INST(0x87)
            CASE_SIMPLE_INST(0x88)
            CASE_SIMPLE_INST(0x89)
            CASE_SIMPLE_INST(0x8a)
            CASE_SIMPLE_INST(0x8b)
            CASE_SIMPLE_INST(0x8c)
            CASE_SIMPLE_INST(0x8d)
            CASE_SIMPLE_INST(0x8e)
            CASE_SIMPLE_INST(0x8f)
            CASE_SIMPLE_INST(0x90)
            CASE_SIMPLE_INST(0x91)
            CASE_SIMPLE_INST(0x92)
            CASE_SIMPLE_INST(0x93)
            CASE_SIMPLE_INST(0x94)
            CASE_SIMPLE_INST(0x95)
            CASE_SIMPLE_INST(0x96)
            CASE_SIMPLE_INST(0x97)
            CASE_SIMPLE_INST(0x98)
            CASE_SIMPLE_INST(0x99)
            CASE_SIMPLE_INST(0x9a)
            CASE_SIMPLE_INST(0x9b)
            CASE_SIMPLE_INST(0x9c)
            CASE_SIMPLE_INST(0x9d)
            CASE_SIMPLE_INST(0x9e)
            CASE_SIMPLE_INST(0x9f)
            CASE_SIMPLE_INST(0xa0)
            CASE_SIMPLE_INST(0xa1)
            CASE_SIMPLE_INST(0xa2)
            CASE_SIMPLE_INST(0xa3)
            CASE_SIMPLE_INST(0xa4)
            CASE_SIMPLE_INST(0xa5)
            CASE_SIMPLE_INST(0xa6)
            CASE_SIMPLE_INST(0xa7)
            CASE_SIMPLE_INST(0xa8)
            CASE_SIMPLE_INST(0xa9)
            CASE_SIMPLE_INST(0xac)
            CASE_SIMPLE_INST(0xad)
            CASE_SIMPLE_INST(0xae)
            CASE_SIMPLE_INST(0xaf)
            CASE_SIMPLE_INST(0xb0)
            CASE_SIMPLE_INST(0xb1)
            CASE_SIMPLE_INST(0xb2)
            CASE_SIMPLE_INST(0xb3)
            CASE_SIMPLE_INST(0xb4)
            CASE_SIMPLE_INST(0xb5)
            CASE_SIMPLE_INST(0xb6)
            CASE_SIMPLE_INST(0xb7)
            CASE_SIMPLE_INST(0xb8)
            CASE_SIMPLE_INST(0xb9)
            CASE_SIMPLE_INST(0xba)
            CASE_SIMPLE_INST(0xbb)
            CASE_SIMPLE_INST(0xbc)
            CASE_SIMPLE_INST(0xbd)
            CASE_SIMPLE_INST(0xbe)
            CASE_SIMPLE_INST(0xbf)
            CASE_SIMPLE_INST(0xc0)
            CASE_SIMPLE_INST(0xc1)
            CASE_SIMPLE_INST(0xc2)
            CASE_SIMPLE_INST(0xc3)
            CASE_SIMPLE_INST(0xc5)
            CASE_SIMPLE_INST(0xc6)
            CASE_SIMPLE_INST(0xc7)
            CASE_SIMPLE_INST(0xc8)
            CASE_SIMPLE_INST(0xc9)
        case 0xc4:
        case 0xaa:
        case 0xab:
        default:
            // skip unimplemented instruction
            std::cerr << "Warning: instruction 0x" << std::hex << std::setw(2) << std::setfill('0') << (int) opcode
                      << " (" << (opcode <= 0xca ? InstructionMnemonics[opcode] : "") << ") is not implemented."
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