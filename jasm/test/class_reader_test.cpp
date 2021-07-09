/**
 * @file class_reader_test.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
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
    is.close();

    std::cout << clazz;

    std::ofstream os("jasm/test/classes/HelloWorld.copy.class");
    clazz.emit_bytecode(os);
    os.close();

    return 0;
}