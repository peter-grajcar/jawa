/**
 * @file class_reader_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iostream>
#include <ios>

#include "class.hpp"

int main()
{
    std::ifstream is("jasm/test/classes/HelloWorld.class",
                     std::ios::in | std::ios::binary);
    jasm::Class clazz(is);

    std::cout << clazz;

    return 0;
}