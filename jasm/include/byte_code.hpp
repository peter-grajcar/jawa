/**
 * @file byte_code.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_BYTE_CODE_HPP
#define JAWA_BYTE_CODE_HPP

#include <bit>
#include <fstream>
#include <iostream>

#define U2_HIGH(X) ((jasm::u1)((X & 0xFF00u) >> 8u))
#define U2_LOW(X) ((jasm::u1)(X & 0x00FFu))
#define U2_SPLIT(X) U2_HIGH(X), U2_LOW(X)

namespace jasm::byte_code {

    using u1 = uint8_t;
    using u2 = uint16_t;
    using u4 = uint32_t;
    using u8 = uint64_t;

    using utf8 = std::string;

    constexpr u8 Magic = 0xCAFEBABE;

    constexpr char BooleanTypePrefix = 'Z';
    constexpr char ByteTypePrefix = 'B';
    constexpr char CharTypePrefix = 'C';
    constexpr char ShortTypePrefix = 'S';
    constexpr char IntTypePrefix = 'I';
    constexpr char LongTypePrefix = 'J';
    constexpr char FloatTypePrefix = 'F';
    constexpr char DoubleTypePrefix = 'D';
    constexpr char ClassTypePrefix = 'L';
    constexpr char ArrayTypePrefix = '[';
    constexpr char VariableTypePrefix = 'T';
    constexpr char VoidTypePrefix = 'V';

    /**
     * Swaps endianness of the given value.
     *
     * @tparam T type of the value.
     * @param src source value.
     * @return source value with swapped endianness.
     */
    template<typename T>
    T
    swap_endianness(T src)
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
    template<typename T>
    T
    read_big_endian(std::istream &is)
    {
        T dst;
        is.read(reinterpret_cast<char *>(&dst), sizeof(T));
#ifdef IS_LITTLE_ENDIAN
        dst = swap_endianness(dst);
#endif
        return dst;
    }

    /**
     * Writes value converted to big endian (if the conversion is necessary) value to the stream.
     *
     * @tparam T value type.
     * @param os output stream.
     * @param val value to write.
     */
    template<typename T>
    void
    write_big_endian(std::ostream &os, T val)
    {
#ifdef IS_LITTLE_ENDIAN
        val = swap_endianness(val);
#endif
        os.write(reinterpret_cast<char *>(&val), sizeof(T));
    }

}

#endif // JAWA_BYTE_CODE_HPP
