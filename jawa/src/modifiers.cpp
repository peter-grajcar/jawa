/**
 * @file modifiers.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "modifiers.hpp"

namespace jawa
{

    gender get_gender(Name name)
    {
        switch (name.back()) {
            case 'a':
                return gender::FEM;
            case 'y':
                return gender::MASC;
            default:
                return gender::NEUT;
        }
    }

}
