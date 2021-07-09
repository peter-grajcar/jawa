/**
 * @file class_reader_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */

#include <iostream>
#include <ios>

#include "class.hpp"

int main()
{
    std::ifstream is("jasm/test/classes/HelloWorld.copy.class",
                     std::ios::in | std::ios::binary);
    jasm::Class clazz(is);
    is.close();

    std::cout << clazz;

    std::ofstream os("jasm/test/classes/HelloWorld.copy.class");
    clazz.emit_bytecode(os);
    os.close();

    return 0;
}