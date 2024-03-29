/**
 * @file method.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <iostream>

#include "method.hpp"

namespace jasm {

    void
    Method::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << ".method ";

        os << (access_flags_ & ACC_PRIVATE ? "private " : "") << (access_flags_ & ACC_PROTECTED ? "protected " : "")
           << (access_flags_ & ACC_PUBLIC ? "public " : "") << (access_flags_ & ACC_STATIC ? "static " : "")
           << (access_flags_ & ACC_FINAL ? "final " : "") << (access_flags_ & ACC_ABSTRACT ? "abstract " : "")
           << (access_flags_ & ACC_NATIVE ? "native " : "") << (access_flags_ & ACC_SYNCHRONIZED ? "synchronized " : "")
           << (access_flags_ & ACC_STRICT ? "strictfp " : "");

        if (pool) {
            auto name_const = dynamic_cast<const Utf8Constant *>(pool->get(name_index_));
            auto descriptor_const = dynamic_cast<const Utf8Constant *>(pool->get(descriptor_index_));
            assert(name_const != nullptr && descriptor_const != nullptr);
            os << name_const->value() << descriptor_const->value() << std::endl;
        } else {
            os << '#' << name_index_ << "(#" << descriptor_index_ << ')' << std::endl;
        }

        for (auto &attr : attributes()) {
            attr->jasm(os, pool);
        }

        os << ".end method" << std::endl;
    }

    void
    Method::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u2>(os, access_flags_);
        write_big_endian<u2>(os, name_index_);
        write_big_endian<u2>(os, descriptor_index_);
        write_big_endian<u2>(os, attributes_.size());
        for (auto &attr : attributes_)
            attr->emit_bytecode(os);
    }

}
