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
        for (u2 i = 0; i < constant_pool_count; ++i) {
            u1 tag = read_big_endian<u1>(is);
            read_constant(is, tag);
        }
    }

    void Class::read_constant(std::istream &is, u1 tag)
    {
        switch (tag) {
            case ConstantPool::CONSTANT_UTF_8:
                // TODO:
                break;
            case ConstantPool::CONSTANT_INTEGER:
                // TODO:
                break;
            case ConstantPool::CONSTANT_FLOAT:
                // TODO:
                break;
            case ConstantPool::CONSTANT_LONG:
                // TODO:
                break;
            case ConstantPool::CONSTANT_DOUBLE:
                // TODO:
                break;
            case ConstantPool::CONSTANT_CLASS:
                // TODO:
                break;
            case ConstantPool::CONSTANT_STRING:
                // TODO:
                break;
            case ConstantPool::CONSTANT_FIELD_REF:
                // TODO:
                break;
            case ConstantPool::CONSTANT_METHOD_REF:
                // TODO:
                break;
            case ConstantPool::CONSTANT_INTERFACE_METHOD_REF:
                // TODO:
                break;
            case ConstantPool::CONSTANT_NAME_AND_TYPE:
                // TODO:
                break;
            case ConstantPool::CONSTANT_METHOD_HANDLE:
                // TODO:
                break;
            case ConstantPool::CONSTANT_METHOD_TYPE:
                // TODO:
                break;
            case ConstantPool::CONSTANT_INVOKE_DYNAMIC:
                // TODO:
                break;
            default:
                assert(false);
        }
    }

}