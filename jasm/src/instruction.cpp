/**
 * @file instruction.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "instruction.hpp"

namespace jasm
{

    template <>
    void GetStatic::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

    template <>
    void InvokeVirtual::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

    template <>
    void InvokeSpecial::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

}
