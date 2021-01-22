/**
 * @file byte_code.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_BYTE_CODE_HPP
#define JAWA_BYTE_CODE_HPP

namespace jasm::byte_code
{

    using u1 = uint8_t;
    using u2 = uint16_t;
    using u4 = uint32_t;

    constexpr char BOOLEAN_TYPE_PREFIX = 'Z';
    constexpr char BYTE_TYPE_PREFIX = 'B';
    constexpr char CHAR_TYPE_PREFIX = 'C';
    constexpr char SHORT_TYPE_PREFIX = 'S';
    constexpr char INTEGER_TYPE_PREFIX = 'I';
    constexpr char LONG_TYPE_PREFIX = 'J';
    constexpr char FLOAT_TYPE_PREFIX = 'F';
    constexpr char DOUBLE_TYPE_PREFIX = 'D';
    constexpr char CLASS_TYPE_PREFIX = 'L';
    constexpr char ARRAY_TYPE_PREFIX = '[';
    constexpr char VARIABLE_TYPE_PREFIX = 'T';

}

#endif //JAWA_BYTE_CODE_HPP
