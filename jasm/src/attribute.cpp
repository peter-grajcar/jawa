/**
 * @file attribute.cpp
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
        // TODO
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
        // TODO:
        return 0;
    }

    void CodeAttribute::emit_bytecode(std::ostream &os) const
    {
        // TODO
    }
}
