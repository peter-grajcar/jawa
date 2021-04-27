/**
 * @file constant.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "constant.hpp"
#include "constant_pool.hpp"

namespace jasm
{

    u1 Utf8Constant::tag() const
    {
        return ConstantPool::CONSTANT_UTF_8;
    }

    u1 IntegerConstant::tag() const
    {
        return ConstantPool::CONSTANT_INTEGER;
    }

    u1 LongConstant::tag() const
    {
        return ConstantPool::CONSTANT_LONG;
    }

    u1 FloatConstant::tag() const
    {
        return ConstantPool::CONSTANT_FLOAT;
    }

    u1 DoubleConstant::tag() const
    {
        return ConstantPool::CONSTANT_DOUBLE;
    }

    u1 ClassConstant::tag() const
    {
        return ConstantPool::CONSTANT_CLASS;
    }

    u1 StringConstant::tag() const
    {
        return ConstantPool::CONSTANT_STRING;
    }

    u1 FieldRefConstant::tag() const
    {
        return ConstantPool::CONSTANT_FIELD_REF;
    }

    u1 MethodRefConstant::tag() const
    {
        return ConstantPool::CONSTANT_METHOD_REF;
    }

    u1 InterfaceMethodRefConstant::tag() const
    {
        return ConstantPool::CONSTANT_INTERFACE_METHOD_REF;
    }

    u1 NameAndTypeConstant::tag() const
    {
        return ConstantPool::CONSTANT_NAME_AND_TYPE;
    }

    u1 MethodHandleConstant::tag() const
    {
        return ConstantPool::CONSTANT_METHOD_HANDLE;
    }

    u1 MethodTypeConstant::tag() const
    {
        return ConstantPool::CONSTANT_METHOD_TYPE;
    }

    u1 InvokeDynamicConstant::tag() const
    {
        return ConstantPool::CONSTANT_INVOKE_DYNAMIC;
    }

}
