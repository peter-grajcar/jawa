/**
 * @file constant.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
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
        Constant() = default;

        Constant(const Constant &) = delete;

        Constant(Constant &&) = default;

        Constant &operator=(const Constant &) = delete;

        Constant &operator=(Constant &&) = default;

        virtual ~Constant() = default;

        virtual void jasm(std::ostream &os) const = 0;

        virtual void emit_bytecode(std::ostream &os) const = 0;

        virtual u1 tag() const = 0;

    };

    class ClassConstant : public Constant
    {
    private:
        u2 name_index_;
    public:
        ClassConstant(u2 name_index) : name_index_(name_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Class" << '#' << name_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, name_index_);
        }

        u1 tag() const override;
    };

    class FieldRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        FieldRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "FieldRef" << '#' << class_index_ << '.'
               << '#' << name_and_type_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, class_index_);
            write_big_endian<u2>(os, name_and_type_index_);
        }

        u1 tag() const override;
    };

    class MethodRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        MethodRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "MethodRef" << '#' << class_index_ << '.'
               << '#' << name_and_type_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, class_index_);
            write_big_endian<u2>(os, name_and_type_index_);
        }

        u1 tag() const override;
    };

    class InterfaceMethodRefConstant : public Constant
    {
    private:
        u2 class_index_;
        u2 name_and_type_index_;
    public:
        InterfaceMethodRefConstant(u2 class_index, u2 name_and_type_index)
                : class_index_(class_index), name_and_type_index_(name_and_type_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "InterfaceMethodRef" << '#' << class_index_
               << '.' << '#' << name_and_type_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, class_index_);
            write_big_endian<u2>(os, name_and_type_index_);
        }

        u1 tag() const override;
    };

    class StringConstant : public Constant
    {
    private:
        u2 string_index_;
    public:
        StringConstant(u2 string_index) : string_index_(string_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "String" << '#' << string_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, string_index_);
        }

        u1 tag() const override;
    };

    class IntegerConstant : public Constant
    {
    private:
        u4 bytes_;
    public:
        IntegerConstant(u4 bytes) : bytes_(bytes) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Integer" << std::hex << bytes_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u4>(os, bytes_);
        }

        u1 tag() const override;
    };

    class FloatConstant : public Constant
    {
    private:
        u4 bytes_;
    public:
        FloatConstant(u4 bytes) : bytes_(bytes) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Float" << std::hex << bytes_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u4>(os, bytes_);
        }

        u1 tag() const override;
    };

    class LongConstant : public Constant
    {
    private:
        u4 high_bytes_;
        u4 low_bytes_;
    public:
        LongConstant(u4 high_bytes, u4 low_bytes)
                : high_bytes_(high_bytes), low_bytes_(low_bytes) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Long" << std::hex << high_bytes_ << low_bytes_
               << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u4>(os, high_bytes_);
            write_big_endian<u4>(os, low_bytes_);
        }

        u1 tag() const override;
    };

    class DoubleConstant : public Constant
    {
    private:
        u4 high_bytes_;
        u4 low_bytes_;
    public:
        DoubleConstant(u4 high_bytes, u4 low_bytes)
                : high_bytes_(high_bytes), low_bytes_(low_bytes) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Double" << std::hex << high_bytes_ << low_bytes_
               << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u4>(os, high_bytes_);
            write_big_endian<u4>(os, low_bytes_);
        }

        u1 tag() const override;
    };

    class NameAndTypeConstant : public Constant
    {
    private:
        u2 name_index_;
        u2 descriptor_index_;
    public:
        NameAndTypeConstant(u2 name_index, u2 descriptor_index)
                : name_index_(name_index), descriptor_index_(descriptor_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "NameAndType" << '#' << name_index_ << ':' << '#'
               << descriptor_index_
               << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, name_index_);
            write_big_endian<u2>(os, descriptor_index_);
        }

        u1 tag() const override;
    };

    class Utf8Constant : public Constant
    {
    private:
        utf8 value_;
    public:
        Utf8Constant(u2 length, u1 bytes[length]) : value_(reinterpret_cast<char *>(bytes),
                                                           length) {};

        Utf8Constant(const char *str) : value_(str) {};

        Utf8Constant(utf8 &str) : value_(str) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "Utf8" << value_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, value_.length());
            for (char ch : value_)
                write_big_endian<u1>(os, ch);
        }

        const std::string &value() const
        {
            return value_;
        }

        u1 tag() const override;
    };

    class MethodHandleConstant : public Constant
    {
    private:
        u2 reference_kind_;
        u2 reference_index_;
    public:
        MethodHandleConstant(u2 reference_kind, u2 reference_index)
                : reference_kind_(reference_kind), reference_index_(reference_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "MethodHandle" << '#' << reference_kind_ << ':'
               << '#' << reference_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, reference_kind_);
            write_big_endian<u2>(os, reference_index_);
        }

        u1 tag() const override;
    };

    class MethodTypeConstant : public Constant
    {
    private:
        u2 descriptor_index_;
    public:
        MethodTypeConstant(u2 descriptor_index) : descriptor_index_(descriptor_index) {};

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << std::left << "MethodType" << '#' << descriptor_index_
               << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, descriptor_index_);
        }

        u1 tag() const override;
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

        void jasm(std::ostream &os) const override
        {
            os << std::setw(20) << "InvokeDynamic" << '#' << bootstrap_method_attr_index_ << ':'
               << '#' << name_and_type_index_ << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, tag());
            write_big_endian<u2>(os, bootstrap_method_attr_index_);
            write_big_endian<u2>(os, name_and_type_index_);
        }

        u1 tag() const override;
    };

}

#endif //JAWA_CONSTANT_HPP
