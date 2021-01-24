/**
 * @file class.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CLASS_HPP
#define JAWA_CLASS_HPP

#include "constant_pool.hpp"

namespace jasm
{

    class Class
    {
    private:
        ConstantPool constant_pool_;

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

    public:
        Class(std::istream &is)
        {
            read_class(is);
        }
    };

}

#endif //JAWA_CLASS_HPP
