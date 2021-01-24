/**
 * @file byte_code.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_BYTE_CODE_HPP
#define JAWA_BYTE_CODE_HPP

#include <bit>
#include <iostream>
#include <fstream>

namespace jasm::byte_code
{

    using u1 = uint8_t;
    using u2 = uint16_t;
    using u4 = uint32_t;
    using u8 = uint64_t;

    constexpr u8 MAGIC = 0xCAFEBABE;

    constexpr char BOOLEAN_TYPE_PREFIX = 'Z';
    constexpr char BYTE_TYPE_PREFIX = 'B';
    constexpr char CHAR_TYPE_PREFIX = 'C';
    constexpr char SHORT_TYPE_PREFIX = 'S';
    constexpr char INT_TYPE_PREFIX = 'I';
    constexpr char LONG_TYPE_PREFIX = 'J';
    constexpr char FLOAT_TYPE_PREFIX = 'F';
    constexpr char DOUBLE_TYPE_PREFIX = 'D';
    constexpr char CLASS_TYPE_PREFIX = 'L';
    constexpr char ARRAY_TYPE_PREFIX = '[';
    constexpr char VARIABLE_TYPE_PREFIX = 'T';

    /**
     * Swaps endianness of the given value.
     *
     * @tparam T type of the value.
     * @param src source value.
     * @return source value with swapped endianness.
     */
    template <typename T>
    T swap_endianness(T src)
    {
        if (sizeof(T) == 1)
            return src;
        T dst = 0;
        for (size_t i = 0; i < sizeof(T); ++i, src >>= 8)
            dst |= (src & 0xFF) << (8 * (sizeof(T) - i - 1));
        return dst;
    }

    /**
     * Reads big endian value from the stream and returns value converted to the system endianness.
     *
     * @tparam T type of the value to read.
     * @param is stream to read from.
     * @return value
     */
    template <typename T>
    T read_big_endian(std::istream &is)
    {
        T dst;
        is.read((char *) &dst, sizeof(T));
#ifdef IS_LITTLE_ENDIAN
        dst = swap_endianness(dst);
#endif
        return dst;
    }


}

#endif //JAWA_BYTE_CODE_HPP
