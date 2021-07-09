/**
 * @file modifiers.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_MODIFIERS_HPP
#define JAWA_MODIFIERS_HPP

#include "context.hpp"

namespace jawa
{

    enum class Gender
    {
        FEM,
        MASC,
        NEUT,
    };

    Gender get_gender(Name name);

    enum class Modifier
    {
        PUBLIC = 0,
        PROTECTED = 1,
        PRIVATE = 2,
        STATIC = 3,
        ABSTRACT = 4,
        FINAL = 5,
        NATIVE = 6,
        SYNCHRONIZED = 7,
        TRANSIENT = 8,
        VOLATILE = 9,
        STRICTFP = 10,
    };

    class ModifierPack
    {
    private:
        uint32_t mod_flags_masc_;
        uint32_t mod_flags_fem_;
    public:
        void set(Modifier mod, Gender gender = Gender::NEUT);

        bool is_set(Modifier mod, Gender gender) const;
    };

    struct ModifierAndAnnotationPack
    {
        ModifierPack modifier_pack_;
        // TODO: AnnotationPack annotation_pack_;
    };

}

#endif //JAWA_MODIFIERS_HPP
