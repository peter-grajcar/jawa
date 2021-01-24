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

        virtual void jasm(std::ostream &os) = 0;
    };

    class ClassConstant : public Constant
    {
    private:
        u2 name_index_;
    public:
        ClassConstant(u2 name_index) : name_index_(name_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Class" << '#' << name_index_ << std::endl;
        }
    };

    class FieldRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        FieldRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "FieldRef" << '#' << class_index_ << '.'
               << '#' << name_and_type_index_ << std::endl;
        }
    };

    class MethodRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        MethodRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "MethodRef" << '#' << class_index_ << '.'
               << '#' << name_and_type_index_ << std::endl;
        }
    };

    class InterfaceMethodRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        InterfaceMethodRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "InterfaceMethodRef" << '#' << class_index_
               << '.' << '#' << name_and_type_index_ << std::endl;
        }
    };

    class StringConstant : public Constant
    {
    private:
        u2 string_index_;
    public:
        StringConstant(u2 string_index) : string_index_(string_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "String" << '#' << string_index_ << std::endl;
        }
    };

    class IntegerConstant : public Constant
    {
    private:
        u4 bytes_;
    public:
        IntegerConstant(u4 bytes) : bytes_(bytes) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Integer" << std::hex << bytes_ << std::endl;
        }
    };

    class FloatConstant : public Constant
    {
    private:
        u4 bytes_;
    public:
        FloatConstant(u4 bytes) : bytes_(bytes) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Float" << std::hex << bytes_ << std::endl;
        }
    };

    class LongConstant : public Constant
    {
    private:
        u4 high_bytes_;
        u4 low_bytes_;
    public:
        LongConstant(u4 high_bytes, u4 low_bytes)
                : high_bytes_(high_bytes), low_bytes_(low_bytes) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Long" << std::hex << high_bytes_ << low_bytes_
               << std::endl;
        }
    };

    class DoubleConstant : public Constant
    {
    private:
        u4 high_bytes_;
        u4 low_bytes_;
    public:
        DoubleConstant(u4 high_bytes, u4 low_bytes)
                : high_bytes_(high_bytes), low_bytes_(low_bytes) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Double" << std::hex << high_bytes_ << low_bytes_
               << std::endl;
        }
    };

    class NameAndTypeConstant : public Constant
    {
    private:
        u2 name_index_;
        u2 descriptor_index_;
    public:
        NameAndTypeConstant(u2 name_index, u2 descriptor_index)
                : name_index_(name_index), descriptor_index_(descriptor_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "NameAndType" << '#' << name_index_ << ':' << '#'
               << descriptor_index_
               << std::endl;
        }
    };

    class Utf8Constant : public Constant
    {
    private:
        std::string value;
    public:
        Utf8Constant(u2 length, u1 bytes[length]) : value(reinterpret_cast<char *>(bytes),
                                                          length) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "Utf8" << value << std::endl;
        }
    };

    class MethodHandleConstant : public Constant
    {
    private:
        u2 reference_kind_;
        u2 reference_index_;
    public:
        MethodHandleConstant(u2 reference_kind, u2 reference_index)
                : reference_kind_(reference_kind), reference_index_(reference_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "MethodHandle" << '#' << reference_kind_ << ':'
               << '#' << reference_index_ << std::endl;
        }
    };

    class MethodTypeConstant : public Constant
    {
    private:
        u2 descriptor_index_;
    public:
        MethodTypeConstant(u2 descriptor_index) : descriptor_index_(descriptor_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << std::left << "MethodType" << '#' << descriptor_index_
               << std::endl;
        }
    };

    class InvokeDynamicConstant : public Constant
    {
    private:
        u2 bootstrap_method_attr_index_;
        u2 name_and_type_index_;
    public:
        InvokeDynamicConstant(u2 bootstrap_method_attr_index, u2 name_and_type_index)
                : bootstrap_method_attr_index_(bootstrap_method_attr_index),
                  name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) override
        {
            os << std::setw(20) << "InvokeDynamic" << '#' << bootstrap_method_attr_index_ << ':'
               << '#' << name_and_type_index_ << std::endl;
        }
    };

}

#endif //JAWA_CONSTANT_HPP
