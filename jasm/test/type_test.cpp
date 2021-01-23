/**
 * @file type_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include <iostream>
#include "type.hpp"

using namespace jasm;

int main()
{
    PrimitiveType i = IntType();
    std::cout << i.descriptor() << std::endl;

    ArrayType i_arr = ArrayType(&i, 2);
    std::cout << i_arr.descriptor() << std::endl;

    ClassType ref = ClassType("java/lang/Thread");
    std::cout << ref.descriptor() << std::endl;

    ArrayType ref_arr = ArrayType(&ref, 1);
    std::cout << ref_arr.descriptor() << std::endl;

    return 0;
}