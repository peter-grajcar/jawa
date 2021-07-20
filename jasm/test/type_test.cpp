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
    MethodType main_signature(&void_type, &str_arr);
    std::cout << main_signature.descriptor() << std::endl;


    std::cout << "int == void           " << (IntType() == VoidType()) << std::endl;
    std::cout << "int == int            " << (IntType() == IntType()) << std::endl;
    std::cout << "Thread == Thread      " << (ref == ClassType("java/lang/Thread")) << std::endl;
    std::cout << "Thread == String      " << (ref == ClassType("java/lang/String")) << std::endl;
    std::cout << "Thread == int         " << (ref == IntType()) << std::endl;
    std::cout << "Thread[] == Thread[]  " << (ref_arr == ArrayType(&ref, 1)) << std::endl;
    std::cout << "void(String[]) == void(String[])  " << (main_signature == MethodType(&void_type, &str_arr))
              << std::endl;
    std::cout << "void(String[]) == void(String)    " << (main_signature == MethodType(&void_type, &str))
              << std::endl;


    return 0;
}