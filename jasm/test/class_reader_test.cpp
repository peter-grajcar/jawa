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
    std::ifstream is("jasm/test/classes/HelloWorld.class",
                     std::ios::in | std::ios::binary);
    jasm::Class clazz(is);

    // dump the constant pool
    size_t i = 1;
    for (auto &c : clazz.constant_pool()) {
        std::cout << '#' << std::setw(4) << std::left << i++ << " = ";
        c->jasm(std::cout);
    }

    std::cout << '{' << std::endl;

    // TODO:

    std::cout << '}' << std::endl;

    for (auto &attr : clazz.attributes()) {
        attr->jasm(std::cout);
    }

    return 0;
}