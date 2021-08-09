/**
 * @file instruction.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "instruction.hpp"

namespace jasm {

    template<>
    void
    GetStatic::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

    template<>
    void
    InvokeVirtual::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

    template<>
    void
    InvokeStatic::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }
    template<>
    void
    InvokeSpecial::jasm(std::ostream &os, const ConstantPool *pool) const
    {
        os << std::setw(20) << mnemonic();
        u2 cp_index = operands_[0];
        cp_index <<= 8u;
        cp_index |= operands_[1];
        os << '#' << cp_index << std::endl;
    }

}
