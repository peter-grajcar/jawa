/**
 * @file field.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_FIELD_HPP
#define JAWA_FIELD_HPP

#include "byte_code.hpp"
#include "attribute.hpp"
#include "constant_pool.hpp"

namespace jasm
{
    using namespace byte_code;

    class Field : public Attributable
    {
    private:
        u2 access_flags_;
        u2 name_index_;
        u2 descriptor_index_;
    public:
        enum AccessFlag : u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_VOLATILE = 0x0040,
            ACC_TRANSIENT = 0x0080,
            ACC_SYNTHETIC = 0x1000,
            ACC_ENUM = 0x4000,
        };

        Field(u2 access_flags, u2 name_index, u2 descriptor_index)
                : access_flags_(access_flags), name_index_(name_index),
                  descriptor_index_(descriptor_index) {}

        void jasm(std::ostream &os, ConstantPool *pool = nullptr) const;
    };

}

#endif //JAWA_FIELD_HPP