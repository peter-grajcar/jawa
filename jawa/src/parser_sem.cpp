/**
 * @file parser_sem.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) 2021 Peter Grajcar
 */

#include "parser_sem.hpp"
#include <iostream>
#include "class.hpp"

namespace jawa
{

    void enter_class(context *ctx, Name &class_name)
    {
        std::cout << "entering class " << class_name << std::endl;
        ctx->new_class_builder(class_name);
    }

    void leave_class(context *ctx)
    {
        std::cout << "leaving class" << std::endl;
        jasm::Class clazz = ctx->class_builder().build();
        std::cout << clazz;
    }

    void enter_method(context *ctx, MethodSignature &signature)
    {
        std::cout << "entering method " << signature.name << std::endl;
        ctx->class_builder().enter_method(signature.name, signature.type);
    }

    void leave_method(context *ctx)
    {
        std::cout << "leaving method";
        ctx->class_builder().leave_method();
    }

}