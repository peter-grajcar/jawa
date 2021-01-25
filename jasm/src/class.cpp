/**
 * @file class_reader.cpp
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
        assert(magic == MAGIC);
        u2 minor_version = read_big_endian<u2>(is);
        u2 major_version = read_big_endian<u2>(is);
        u2 constant_pool_count = read_big_endian<u2>(is);

        // The value of the constant_pool_count item is equal to the number of entries in the
        // constant_pool table plus one
        assert(constant_pool_count > 0);

        for (u2 i = 1; i < constant_pool_count; ++i) {
            u1 tag = read_big_endian<u1>(is);
            read_constant(is, tag);
        }

        u2 access_flags = read_big_endian<u2>(is);
        u2 this_class = read_big_endian<u2>(is);
        u2 super_class = read_big_endian<u2>(is);
        u2 interfaces_count = read_big_endian<u2>(is);

        for (u2 i = 0; i < interfaces_count; ++i) {
            u2 interface_index = read_big_endian<u2>(is);
            assert(dynamic_cast<ClassConstant *>(constant_pool_[interface_index].get()) != nullptr);
            // TODO:
        }

        u2 fields_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < fields_count; ++i) {
            u2 field_access_flags = read_big_endian<u2>(is);
            u2 name_index = read_big_endian<u2>(is);
            u2 descriptor_index = read_big_endian<u2>(is);

            auto field = std::make_unique<Field>(field_access_flags, name_index, descriptor_index);

            u2 attributes_count = read_big_endian<u2>(is);
            for (u2 j = 0; j < attributes_count; ++j) {
                read_attribute(is, field.get());
            }

            add_field(field);
        }

        u2 methods_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < methods_count; ++i) {
            u2 method_access_flags = read_big_endian<u2>(is);
            u2 name_index = read_big_endian<u2>(is);
            u2 descriptor_index = read_big_endian<u2>(is);

            auto method = std::make_unique<Method>(method_access_flags, name_index,
                                                   descriptor_index);

            u2 attributes_count = read_big_endian<u2>(is);
            for (u2 j = 0; j < attributes_count; ++j) {
                read_attribute(is, method.get());
            }

            add_method(method);
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

    void Class::read_attribute(std::istream &is, Attributable *atr)
    {
        u2 attribute_name_index = read_big_endian<u2>(is);
        u4 attribute_length = read_big_endian<u4>(is);

        auto *attribute_name_const = dynamic_cast<Utf8Constant *>(
                constant_pool_[attribute_name_index].get()
        );
        assert(attribute_name_const != nullptr);

        std::string attribute_name = attribute_name_const->value();

        if (attribute_name == "SourceFile") {
            u2 source_file_index = read_big_endian<u2>(is);
            atr->make_attribute<SourceFileAttribute>(attribute_name_index, source_file_index);
        }

        // debug only, delete later
        for (u4 i = 0; i < attribute_length; ++i) {
            read_big_endian<u1>(is);
        }
    }

}