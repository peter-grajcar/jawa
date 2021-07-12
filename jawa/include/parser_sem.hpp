/**
 * @file parser_sem.cpp.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_PARSER_SEM_CPP_HPP
#define JAWA_PARSER_SEM_CPP_HPP

#include "context.hpp"

namespace jawa
{

    void enter_class(context *ctx, Name &class_name);

    void leave_class(context *ctx);

    struct MethodSignature
    {
        jasm::MethodSignatureType type;
        Name name;

        // TODO: constructor
        // MethodSignature() : name(), type() {};
    };

    void enter_method(context *ctx, MethodSignature &signature);

    void leave_method(context *ctx);

}

#endif //JAWA_PARSER_SEM_CPP_HPP
