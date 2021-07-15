/**
 * @file method.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_METHOD_HPP
#define JAWA_METHOD_HPP

#include "attribute.hpp"
#include "constant_pool.hpp"

namespace jasm
{

    class Method : public Attributable
    {
    private:
        u2 access_flags_;
        u2 name_index_;
        u2 descriptor_index_;
    public:
        enum AccessFlag
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_SYNCHRONIZED = 0x0020,
            ACC_BRIDGE = 0x0040,
            ACC_VARARGS = 0x0080,
            ACC_NATIVE = 0x0100,
            ACC_ABSTRACT = 0x0400,
            ACC_STRICT = 0x0800,
            ACC_SYNTHETIC = 0x1000,
        };

        Method(u2 access_flags, u2 name_index, u2 descriptor_index)
                : access_flags_(access_flags), name_index_(name_index),
                  descriptor_index_(descriptor_index) {}


        inline u2 access_flags() const
        {
            return access_flags_;
        }

        inline u2 name_index() const
        {
            return name_index_;
        }

        inline u2 descriptor_index() const
        {
            return descriptor_index_;
        }

        void jasm(std::ostream &os, const ConstantPool *pool = nullptr) const;

        void emit_bytecode(std::ostream &os) const;
    };

}

#endif //JAWA_METHOD_HPP
