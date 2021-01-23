/**
 * @file class_reader_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iostream>
#include <fstream>
#include <ios>

#include "byte_code/byte_code.hpp"


int main()
{
    std::ifstream is("/Users/petergrajcar/Desktop/Test.class", std::ios::in | std::ios::binary);
    jasm::byte_code::read_class(is);
    return 0;
}