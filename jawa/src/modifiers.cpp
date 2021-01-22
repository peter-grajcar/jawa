/**
 * @file modifiers.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "modifiers.hpp"

namespace jawa
{

    Gender get_gender(Name name)
    {
        switch (name.back()) {
            case 'a':
                return Gender::FEM;
            case 'y':
                return Gender::MASC;
            default:
                return Gender::NEUT;
        }
    }

    void ModifierPack::set(Modifier mod, Gender gender)
    {
        uint32_t mask = 1U << (size_t) mod;
        assert((mod_flags_masc_ & mask) == 0 && (mod_flags_fem_ & mask) == 0);
        if (gender == Gender::NEUT || gender == Gender::MASC) {
            mod_flags_masc_ |= mask;
        }
        if (gender == Gender::NEUT || gender == Gender::FEM) {
            mod_flags_fem_ |= mask;
        }
    }

    bool ModifierPack::is_set(Modifier mod, Gender gender) const
    {
        uint32_t mask = 1U << (size_t) mod;
        switch (gender) {
            case Gender::FEM:
                return (mod_flags_fem_ & mask) && !(mod_flags_masc_ & mask);
            case Gender::MASC:
                return !(mod_flags_fem_ & mask) && (mod_flags_masc_ & mask);
            case Gender::NEUT:
                return (mod_flags_fem_ & mask) && (mod_flags_masc_ & mask);
        }
    }


}
