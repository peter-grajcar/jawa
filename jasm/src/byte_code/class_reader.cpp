/**
 * @file class_reader.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include "byte_code/byte_code.hpp"
#include "byte_code/constant_pool.hpp"

namespace jasm::byte_code
{

    void read_class(std::istream &is)
    {
        u4 magic = read_big_endian<u4>(is);
        assert(magic == MAGIC);
        u2 minor_version = read_big_endian<u2>(is);
        u2 major_version = read_big_endian<u2>(is);
        u2 constant_pool_count = read_big_endian<u2>(is);
        for (u2 i = 0; i < constant_pool_count; ++i) {
            u1 tag = read_big_endian<u1>(is);
            // TODO:
        }
    }

}