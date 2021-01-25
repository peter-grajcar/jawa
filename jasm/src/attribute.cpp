/**
 * @file attribute.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "attribute.hpp"

namespace jasm
{

    void SourceFileAttribute::jasm(std::ostream &os, ConstantPool *pool) const
    {
        if (pool) {
            auto source_file_const = dynamic_cast<Utf8Constant *>(pool->get(
                    source_file_index_)
            );
            os << "SourceFile: \"" << source_file_const->value() << '\"' << std::endl;
        } else {
            os << "SourceFile: #" << source_file_index_ << std::endl;
        }
    }

}
