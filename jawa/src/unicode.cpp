/**
 * @file unicode.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "unicode.hpp"

namespace jawa::unicode {

    int
    utf8_length(const char *str)
    {
        int len = 0;
        while (*str)
            len += (*str++ & 0xc0) != 0x80;
        return len;
    }

}
