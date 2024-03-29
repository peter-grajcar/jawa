/**
 * @file format.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_FORMAT_HPP
#define JAWA_FORMAT_HPP

#include <iostream>
#include <string>

/**
 * TODO:
 *
 * @param os output stream.
 * @param fmt format.
 * @return output stream.
 */
std::ostream &
format(std::ostream &os, const char *fmt);

/**
 * TODO:
 *
 * @tparam T type of the first argument.
 * @tparam Args types of the other arguments.
 * @param os output stream.
 * @param fmt format.
 * @param arg first argument.
 * @param args other arguments.
 * @return output stream.
 */
template<typename T, typename... Args>
std::ostream &
format(std::ostream &os, const char *fmt, T arg, Args... args)
{
    while (*fmt && *fmt != '%') {
        os << *fmt;
        ++fmt;
    }

    if (!*fmt)
        return os;
    ++fmt;

    os << arg;
    format(os, fmt, args...);
    return os;
}

#endif // JAWA_FORMAT_HPP
