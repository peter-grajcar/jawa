/**
 * @file endianess_test.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iomanip>
#include <iostream>

#include "byte_code.hpp"

using namespace jasm::byte_code;

int
main()
{
    u4 a = 0x12345678;
    u4 b = swap_endianness(a);
#ifdef IS_LITTLE_ENDIAN
    std::cout << "little endian: " << std::hex << a << std::endl;
    std::cout << "big endian:    " << std::hex << b << std::endl;
    assert(a != b);
#else
    assert(a == b);
#endif
    return 0;
}