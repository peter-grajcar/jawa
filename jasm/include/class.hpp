/**
 * @file class.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CLASS_HPP
#define JAWA_CLASS_HPP

#include <memory>

#include "constant_pool.hpp"
#include "attribute.hpp"
#include "field.hpp"
#include "method.hpp"

namespace jasm
{

    class Class : public Attributable
    {
    private:
        ConstantPool constant_pool_;
        std::vector<std::unique_ptr<Field>> fields_;
        std::vector<std::unique_ptr<Method>> methods_;

        /**
         * Reads a class file from a class file input stream.
         *
         * @param is binary input stream.
         */
        void read_class(std::istream &is);

        /**
         * Reads a single constant from a class file input stream.
         *
         * @param is binary input stream.
         * @param tag constant pool tag.
         * @see ConstantPool::Tag
         */
        void read_constant(std::istream &is, u1 tag);

        void read_attribute(std::istream &is, Attributable *atr);

    public:
        enum AccessFlag : u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_FINAL = 0x0010,
            ACC_SUPER = 0x0020,
            ACC_INTERFACE = 0x0200,
            ACC_ABSTRACT = 0x0400,
            ACC_SYNTHETIC = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM = 0x4000
        };

        Class(std::istream &is)
        {
            read_class(is);
        }

        inline const ConstantPool &constant_pool() const
        {
            return constant_pool_;
        }

        inline ConstantPool &constant_pool()
        {
            return constant_pool_;
        }

        inline void add_field(std::unique_ptr<Field> &field)
        {
            fields_.push_back(std::move(field));
        }

        inline void add_method(std::unique_ptr<Method> &method)
        {
            methods_.push_back(std::move(method));
        }

        inline std::vector<std::unique_ptr<Field>> &fields()
        {
            return fields_;
        }

        inline std::vector<std::unique_ptr<Method>> &methods()
        {
            return methods_;
        }
    };

}

#endif //JAWA_CLASS_HPP
