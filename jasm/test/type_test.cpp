/**
 * @file type_test.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <iostream>
#include "type.hpp"

using namespace jasm;

int main()
{
    PrimitiveType i = IntType();
    std::cout << i.descriptor() << std::endl;

    ArrayType i_arr(&i, 2);
    std::cout << i_arr.descriptor() << std::endl;

    ClassType ref("java/lang/Thread");
    std::cout << ref.descriptor() << std::endl;

    ArrayType ref_arr(&ref, 1);
    std::cout << ref_arr.descriptor() << std::endl;

    ClassType str("java/lang/String");
    ArrayType str_arr(&str, 1);
    PrimitiveType void_type = VoidType();
    MethodSignatureType main_signature(&void_type, &str_arr);
    std::cout << main_signature.descriptor() << std::endl;


    return 0;
}