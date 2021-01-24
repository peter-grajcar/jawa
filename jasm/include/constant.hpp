/**
 * @file constant.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONSTANT_HPP
#define JAWA_CONSTANT_HPP

#include "byte_code.hpp"

namespace jasm
{

    using namespace byte_code;

    class Constant
    {
    public:
        virtual ~Constant() = default;
    };

    class ClassConstant : public Constant
    {
    private:
    public:
        ClassConstant(u2 name_index);
    };

    class FieldRefConstant : public Constant
    {
    private:
    public:
        FieldRefConstant(u2 class_index, u2 name_and_type_index);
    };

    class MethodRefConstant : public Constant
    {
    private:
    public:
        MethodRefConstant(u2 class_index, u2 name_and_type_index);
    };

    class InterfaceMethodRefConstant : public Constant
    {
    private:
    public:
        InterfaceMethodRefConstant(u2 class_index, u2 name_and_type_index);
    };

    class StringConstant : public Constant
    {
    private:
    public:
        StringConstant(u2 string_index);
    };

    class IntegerConstant : public Constant
    {
    private:
    public:
        IntegerConstant(u4 bytes);
    };

    class FloatConstant : public Constant
    {
    private:
    public:
        FloatConstant(u4 bytes);
    };

    class LongConstant : public Constant
    {
    private:
    public:
        LongConstant(u4 high_bytes, u4 low_bytes);
    };

    class DoubleConstant : public Constant
    {
    private:
    public:
        DoubleConstant(u4 high_bytes, u4 low_bytes);
    };

    class NameAndTypeConstant : public Constant
    {
    private:
    public:
        NameAndTypeConstant(u2 name_index, u2 descriptor_index);
    };

    class Utf8Constant : public Constant
    {
    private:
    public:
        Utf8Constant(u2 length, u1 bytes[length]);
    };

    class MethodHandleConstant : public Constant
    {
    private:
    public:
        MethodHandleConstant(u2 reference_kind, u2 reference_index);
    };

    class MethodTypeConstant : public Constant
    {
    private:
    public:
        MethodTypeConstant(u2 descriptor_index);
    };

    class InvokeDynamicConstant : public Constant
    {
    private:
    public:
        InvokeDynamicConstant(u2 bootstrap_method_attr_index, u2 name_and_type_index);
    };

}

#endif //JAWA_CONSTANT_HPP
