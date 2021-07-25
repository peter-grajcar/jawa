/**
 * @file format.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include "format.hpp"

std::ostream &
format(std::ostream &os, const char *fmt)
{
    while (*fmt) {
        if (*fmt != '%')
            os << *fmt;
        ++fmt;
    }
    return os;
}
