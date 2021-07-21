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

    struct Expression
    {
        TypeObs type;

        Expression() : type(nullptr) {};

        explicit Expression(TypeObs type) : type(type) {};
    };

    struct ClassAndName
    {
        Name class_name;
        Name name;
    };

    using ExpressionOpt = std::optional<Expression>;
    using ExpressionArray = std::vector<Expression>;

    void enter_method(context_t ctx, const Name &method_name, TypeObs return_type, TypeObsArray &argument_types);

    void enter_static_initializer(context_t ctx);

    void leave_method(context_t ctx);

    void declare_method(context_t ctx);

    TypeObs find_class(context_t ctx, const Name &name);

    void generate_default_constructor(context_t ctx);

    Expression load_string_literal(context_t ctx, const Name &name);

    ClassAndName resolve_method_class(context_t ctx, const Name &method);

    Expression
    invoke_method(context_t ctx, const Expression &expr, const Name &method_name, const ExpressionArray &arguments);

    Expression invoke_method(context_t ctx, const ClassAndName &method, const ExpressionArray &arguments);

}

#endif //JAWA_PARSER_SEM_CPP_HPP
