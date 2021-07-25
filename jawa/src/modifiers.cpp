/**
 * @file modifiers.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "modifiers.hpp"

namespace jawa {

    ModifierForm get_form(Name name)
    {
        switch (name.back()) {
        case 'a':
            return ModifierForm::FEM;
        case 'y':
            return ModifierForm::MASC;
        default:
            return ModifierForm::NEUT;
        }
    }

    void ModifierPack::set(Modifier mod, ModifierForm form)
    {
        uint32_t mask = 1U << (size_t) mod;
        assert((mod_flags_masc_ & mask) == 0 && (mod_flags_fem_ & mask) == 0);
        if (form == ModifierForm::NEUT || form == ModifierForm::MASC) {
            mod_flags_masc_ |= mask;
        }
        if (form == ModifierForm::NEUT || form == ModifierForm::FEM) {
            mod_flags_fem_ |= mask;
        }
        if (form == ModifierForm::NONE) {
            mod_flags_fem_ &= ~mask;
            mod_flags_masc_ &= ~mask;
        }
    }

    ModifierForm ModifierPack::get(Modifier mod) const
    {
        uint32_t mask = 1U << (size_t) mod;
        if ((mod_flags_masc_ & mask) && (mod_flags_fem_ & mask))
            return ModifierForm::NEUT;
        if (mod_flags_masc_ & mask)
            return ModifierForm::MASC;
        if (mod_flags_fem_ & mask)
            return ModifierForm::FEM;
        return ModifierForm::NONE;
    }

}
