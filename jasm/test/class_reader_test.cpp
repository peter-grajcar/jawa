/**
 * @file class_reader_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iostream>
#include <fstream>
#include <ios>

#include "class.hpp"

int main()
{
    std::ifstream is("/Users/petergrajcar/Desktop/jasmin-2.4/examples/HelloWorld.class",
                     std::ios::in | std::ios::binary);
    jasm::Class c(is);
    return 0;
}