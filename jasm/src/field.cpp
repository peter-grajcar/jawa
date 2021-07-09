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

namespace jasm
{

    void Field::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        // TODO
    }

    void Field::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u2>(os, access_flags_);
        write_big_endian<u2>(os, name_index_);
        write_big_endian<u2>(os, descriptor_index_);
        write_big_endian<u2>(os, attributes_.size());
        for (auto &attr : attributes_)
            attr->emit_bytecode(os);
    }

}
