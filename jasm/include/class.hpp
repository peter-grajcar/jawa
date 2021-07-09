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
        std::vector<Field> fields_;
        std::vector<Method> methods_;
        std::vector<u2> interfaces_;

        u2 minor_version_;
        u2 major_version_;
        u2 access_flags_;
        u2 this_class_;
        u2 super_class_;

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

        /**
         * Reads a single attribute from a class file input stream.
         *
         * @param is binary input stream.
         * @param attr a class that may contain attributes.
         */
        void read_attribute(std::istream &is, Attributable *attr);

        /**
         * Reads a single instruction from a class file input stream.
         *
         * @param is binary input stream.
         * @param code code attribute.
         * @return instruction width.
         */
        u4 read_instruction(std::istream &is, CodeAttribute *code);

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

        explicit Class(std::istream &is)
        {
            read_class(is);
        }

        Class(u2 minor_version, u2 major_version, u2 access_flags)
                : minor_version_(minor_version), major_version_(major_version),
                  access_flags_(access_flags) {}

        /**
        * Writes the class' bytecode to the given output stream.
         *
        * @param os output stream.
        */
        void emit_bytecode(std::ostream &os) const;

        inline const ConstantPool &constant_pool() const
        {
            return constant_pool_;
        }

        inline ConstantPool &constant_pool()
        {
            return constant_pool_;
        }

        inline void add_field(Field &&field)
        {
            fields_.emplace_back(std::forward<Field>(field));
        }

        inline void add_method(Method &&method)
        {
            methods_.emplace_back(std::forward<Method>(method));
        }

        inline std::vector<Field> &fields()
        {
            return fields_;
        }

        inline std::vector<Method> &methods()
        {
            return methods_;
        }

        inline u2 minor_version()
        {
            return minor_version_;
        }

        inline u2 major_version()
        {
            return major_version_;
        }

        inline ClassConstant *this_class()
        {
            return dynamic_cast<ClassConstant *>(constant_pool_.get(this_class_));
        }

        inline ClassConstant *super_class()
        {
            return dynamic_cast<ClassConstant *>(constant_pool_.get(super_class_));
        }

        void jasm(std::ostream &os) const;

        inline friend std::ostream &operator<<(std::ostream &os, const Class &clazz)
        {
            clazz.jasm(os);
            return os;
        }
    };

}

#endif //JAWA_CLASS_HPP
