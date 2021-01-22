/**
 * @file constant_pool.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONSTANT_POOL_HPP
#define JAWA_CONSTANT_POOL_HPP

#include <cinttypes>

#include "byte_code.hpp"

namespace jasm::byte_code::constant_pool
{

    enum class Tag : u1
    {
        UTF_8 = 1,
        INTEGER = 3,
        FLOAT = 4,
        LONG = 5,
        DOUBLE = 6,
        CLASS = 7,
        STRING = 8,
        FIELD_REF = 9,
        METHOD_REF = 10,
        INTERFACE_METHOD_REF = 11,
        NAME_AND_TYPE = 12,
        METHOD_HANDLE = 15,
        METHOD_TYPE = 16,
        INVOKE_DYNAMIC = 18,
    };

    struct ClassInfo
    {
        u1 tag;
        u2 name_index;
    };

    struct FieldRefInfo
    {
        u1 tag;
        u2 class_index;
        u2 name_and_type_index;
    }

    struct MethodRefInfo
    {
        u1 tag;
        u2 class_index;
        u2 name_and_type_index;
    }

    struct InterfaceMethodRefInfo
    {
        u1 tag;
        u2 class_index;
        u2 name_and_type_index;
    }

    struct StringInfo
    {
        u1 tag;
        u2 string_index;
    }

    struct IntegerInfo
    {
        u1 tag;
        u4 bytes;
    }

    struct FloatInfo
    {
        u1 tag;
        u4 bytes;
    }

    struct LongInfo
    {
        u1 tag;
        u4 high_bytes;
        u4 low_bytes;
    }

    struct DoubleInfo
    {
        u1 tag;
        u4 high_bytes;
        u4 low_bytes;
    }

    struct NameAndTypeInfo
    {
        u1 tag;
        u2 name_index;
        u2 descriptor_index;
    }

    struct Utf8Info
    {
        u1 tag;
        u2 length;
        u1 bytes[];
    }

    struct MethodTypeInfo
    {
        u1 tag;
        u2 descriptor_index;
    }

    struct InvokeDynamicInfo
    {
        u1 tag;
        u2 bootstrap_method_attr_index;
        u2 name_and_type_index;
    }

}

#endif //JAWA_CONSTANT_POOL_HPP
