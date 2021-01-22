/**
 * @file format.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include "format.hpp"

std::ostream &format(std::ostream &os, const char *fmt)
{
    while (*fmt) {
        if (*fmt != '%')
            os << *fmt;
        ++fmt;
    }
    return os;
}
