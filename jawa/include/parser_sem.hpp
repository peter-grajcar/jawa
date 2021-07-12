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

    void enter_class(context_t ctx, const Name &class_name);

    void leave_class(context_t ctx);

    struct MethodSignature
    {
        jasm::MethodSignatureType type;
        Name name;

        // TODO: constructor
        // MethodSignature() : name(), type() {};
    };

    void enter_method(context_t ctx, const Name &method_name, TypeObs return_type, TypeObsArray &argument_types);

    void leave_method(context_t ctx);

    TypeObs find_class(context_t ctx, const Name &name);

    void generate_default_constructor(context_t ctx);

}

#endif //JAWA_PARSER_SEM_CPP_HPP
