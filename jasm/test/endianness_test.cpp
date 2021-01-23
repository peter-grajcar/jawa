/**
 * @file endianess_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iostream>
#include <iomanip>

#include "byte_code/byte_code.hpp"

using namespace jasm::byte_code;

int main()
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