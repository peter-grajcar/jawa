/**
 * @file types.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_TYPES_HPP
#define JAWA_TYPES_HPP

#include <cinttypes>
#include <vector>

namespace jawa
{

    using bool_t = bool;
    using byte_t = int8_t;
    using short_t = int16_t;
    using int_t = int32_t;
    using long_t = int64_t;
    using float_t = float;
    using double_t = double;
    using char_t = uint16_t;

    using Name = std::string;
    using NameList = std::vector<Name>;

}

#endif //JAWA_TYPES_HPP
