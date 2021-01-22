/**
 * @file format.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_FORMAT_HPP
#define JAWA_FORMAT_HPP

#include <iostream>
#include <string>

std::ostream &format(std::ostream &os, const char *fmt);

template <typename T, typename ...Args>
std::ostream &format(std::ostream &os, const char *fmt, T arg, Args ...args)
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

#endif //JAWA_FORMAT_HPP
