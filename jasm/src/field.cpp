/**
 * @file field.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "field.hpp"

namespace jasm {

    void
    Field::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << ".field ";

        os << (access_flags_ & ACC_PRIVATE ? "private " : "") << (access_flags_ & ACC_PROTECTED ? "protected " : "")
           << (access_flags_ & ACC_PUBLIC ? "public " : "") << (access_flags_ & ACC_STATIC ? "static " : "")
           << (access_flags_ & ACC_FINAL ? "final " : "") << (access_flags_ & ACC_TRANSIENT ? "transient " : "")
           << (access_flags_ & ACC_VOLATILE ? "volatile " : "") << (access_flags_ & ACC_SYNTHETIC ? "synthetic " : "");

        if (pool) {
            auto name_const = dynamic_cast<const Utf8Constant *>(pool->get(name_index_));
            auto descriptor_const = dynamic_cast<const Utf8Constant *>(pool->get(descriptor_index_));
            assert(name_const != nullptr && descriptor_const != nullptr);
            os << name_const->value() << ' ' << descriptor_const->value() << std::endl;
        } else {
            os << '#' << name_index_ << "(#" << descriptor_index_ << ')' << std::endl;
        }
    }

    void
    Field::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u2>(os, access_flags_);
        write_big_endian<u2>(os, name_index_);
        write_big_endian<u2>(os, descriptor_index_);
        write_big_endian<u2>(os, attributes_.size());
        for (auto &attr : attributes_)
            attr->emit_bytecode(os);
    }

}
