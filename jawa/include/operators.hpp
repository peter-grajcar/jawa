/**
 * @file operators.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_OPERATORS_HPP
#define JAWA_OPERATORS_HPP

namespace jawa::operators {

    enum addop
    {
        ADD,
        SUB
    };

    enum incdec
    {
        INC,
        DEC
    };

    enum divop
    {
        DIV,
        MOD
    };

    enum comp
    {
        COMP_ADD,
        COMP_SUB,
        COMP_MUL,
        COMP_DIV,
        COMP_MOD,
        COMP_SHL,
        COMP_SHR,
        COMP_SHR_U,
        COMP_AND,
        COMP_OR,
        COMP_NOT,
        COMP_XOR
    };

    enum relop
    {
        GTE,
        LTE
    };

    enum eqop
    {
        EQ,
        NE
    };

}

#endif // JAWA_OPERATORS_HPP
