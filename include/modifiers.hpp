/**
 * @file modifiers.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_MODIFIERS_HPP
#define JAWA_MODIFIERS_HPP

#include "context.hpp"

namespace jawa
{

    enum class gender
    {
        FEM,
        MASC,
        NEUT,
    };

    gender get_gender(Name name);

}

#endif //JAWA_MODIFIERS_HPP
