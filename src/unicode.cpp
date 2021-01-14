/**
 * @file unicode.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "unicode.hpp"

namespace jawa::unicode
{

    int utf8_length(const char *str)
    {
        int len = 0;
        while (*str)
            len += (*str++ & 0xc0) != 0x80;
        return len;
    }

}
