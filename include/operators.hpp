/**
 * @file operators.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_OPERATORS_HPP
#define JAWA_OPERATORS_HPP

namespace jawa::operators
{

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

    enum shift
    {
        SHL,
        SHR,
        SHR_U
    };

    enum relop
    {
        LT,
        LTE
    };

    enum eqop
    {
        EQ,
        NE
    };

}

#endif //JAWA_OPERATORS_HPP
