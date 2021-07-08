/**
 * @file builder_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "class.hpp"
#include "builder.hpp"

using namespace jasm;

int main()
{
    Class clazz(59, 0, Class::AccessFlag::ACC_PUBLIC);

    clazz.constant_pool().make_constant<Utf8Constant>("test");

    std::cout << clazz;

    return 0;
}
