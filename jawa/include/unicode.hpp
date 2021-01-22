/**
 * @file unicode.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_UNICODE_HPP
#define JAWA_UNICODE_HPP

#include <locale>

namespace jawa::unicode
{

    /**
     * Computes the length of a string by counting only the first bytes of
     * UTF-8 character codes.
     *
     * @param str string.
     * @return length of the string.
     */
    int utf8_length(const char *str);

}

#endif //JAWA_UNICODE_HPP
