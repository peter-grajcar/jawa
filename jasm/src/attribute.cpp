/**
 * @file attribute.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "attribute.hpp"

namespace jasm
{

    void SourceFileAttribute::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        if (pool) {
            auto source_file_const = dynamic_cast<const Utf8Constant *>(pool->get(
                    source_file_index_)
            );
            os << "SourceFile: \"" << source_file_const->value() << '\"' << std::endl;
        } else {
            os << "SourceFile: #" << source_file_index_ << std::endl;
        }
    }

    void SourceFileAttribute::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u2>(os, attribute_name_index_);
        write_big_endian<u4>(os, 2);
        write_big_endian<u2>(os, source_file_index_);
    }

    void CodeAttribute::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << "  " << std::setw(20) << ".limit stack" << max_stack_ << std::endl;
        os << "  " << std::setw(20) << ".limit locals" << max_locals_ << std::endl;

        for (auto &inst : code_) {
            os << "  "; // indent
            inst->jasm(os, pool);
        }
    }

    u4 CodeAttribute::length() const
    {
        return 12 + exception_table_length() + code_length() + attributes_length();
    }

    u4 CodeAttribute::code_length() const
    {
        u4 code_length = 0;
        for (auto &inst : code_)
            code_length += inst->size();
        return code_length;
    }

    u4 CodeAttribute::exception_table_length() const
    {
        return 8 * exception_table_.size();
    }

    u4 CodeAttribute::attributes_length() const
    {
        u4 attributes_length = 0;
        for (auto &attr : attributes_)
            attributes_length += attr->length();
        return attributes_length;
    }

    void CodeAttribute::emit_bytecode(std::ostream &os) const
    {
        write_big_endian<u2>(os, attribute_name_index_);
        write_big_endian<u4>(os, length());
        write_big_endian<u2>(os, max_stack_);
        write_big_endian<u2>(os, max_locals_);
        write_big_endian<u4>(os, code_length());

        for (auto &inst : code_)
            inst->emit_bytecode(os);

        write_big_endian<u2>(os, exception_table_.size());
        for (auto &entry : exception_table_) {
            write_big_endian<u2>(os, entry.start_pc);
            write_big_endian<u2>(os, entry.end_pc);
            write_big_endian<u2>(os, entry.handler_pc);
            write_big_endian<u2>(os, entry.catch_type);
        }

        write_big_endian<u2>(os, attributes_.size());
        for (auto &attr : attributes_)
            attr->emit_bytecode(os);
    }
}
