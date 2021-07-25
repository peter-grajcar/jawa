/**
 * @file class_reader_test.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include <ios>
#include <iostream>

#include "class.hpp"

int
main()
{
    // std::ifstream is("jasm/test/classes/HelloWorld.class",
    std::ifstream is("/Users/petergrajcar/Desktop/java/io/PrintStream.class", std::ios::in | std::ios::binary);
    jasm::Class clazz(is);
    is.close();

    std::cout << clazz;

    return 0;
}