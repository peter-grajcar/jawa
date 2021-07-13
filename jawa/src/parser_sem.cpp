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

#define BUILDER ctx->class_builder()
#define TYPE_TABLE ctx->type_table()

namespace jawa
{

    void enter_class(context_t ctx, const Name &class_name)
    {
        std::cout << "entering class " << class_name << std::endl;
        ctx->new_class_builder(class_name);
        BUILDER.set_version(59, 0);
    }

    void leave_class(context_t ctx)
    {
        std::cout << "leaving class" << std::endl;

        // TODO: check if no constructor was created
        generate_default_constructor(ctx);

        auto class_name = BUILDER.class_name();
        jasm::Class clazz = BUILDER.build();
        std::cout << clazz;

        std::ofstream os(class_name + ".class");
        clazz.emit_bytecode(os);
        os.close();
    }


    bool is_main(context_t ctx, const Name &method_name, TypeObs return_type, TypeObsArray &argument_types)
    {
        if (method_name != "głowny")
            return false;
        if (return_type != TYPE_TABLE.get_void_type())
            return false;
        if (argument_types.size() != 1)
            return false;
        TypeObs str_type = TYPE_TABLE.get_class_type("java/lang/String");
        TypeObs str_arr_type = TYPE_TABLE.get_array_type(str_type, 1);
        return argument_types[0] == str_arr_type;
    }

    void enter_method(context_t ctx, const Name &method_name, TypeObs return_type, TypeObsArray &argument_types)
    {
        std::cout << "entering method " << method_name << std::endl;
        MethodTypeObs method_type = TYPE_TABLE.get_method_type(return_type, argument_types);
        if (is_main(ctx, method_name, return_type, argument_types)) {
            BUILDER.enter_method("main", *method_type,
                                 jasm::Method::ACC_PUBLIC | jasm::Method::ACC_STATIC // <- temproary TODO: remove
            );
        } else {
            BUILDER.enter_method(method_name, *method_type);
        }
    }

    void leave_method(context_t ctx)
    {
        std::cout << "leaving method" << std::endl;
        BUILDER.make_instruction<jasm::Return>();
        BUILDER.leave_method();
    }

    TypeObs find_class(context_t ctx, const Name &name)
    {
        if (name == "Łańcuch")
            return TYPE_TABLE.get_class_type("java/lang/String");
        return TYPE_TABLE.get_class_type(name);
    }

    void generate_default_constructor(context_t ctx)
    {
        VoidTypeObs void_type = TYPE_TABLE.get_void_type();
        MethodTypeObs void_method_type = TYPE_TABLE.get_method_type(void_type, TypeObsArray());
        BUILDER.enter_constructor(*void_method_type, jasm::Method::ACC_PUBLIC);
        BUILDER.make_instruction<jasm::RefLoad0>();
        jasm::u2 object_constructor = BUILDER.add_method_constant("java/lang/Object", "<init>", *void_method_type);
        BUILDER.make_instruction<jasm::InvokeSpecial>(U2_SPLIT(object_constructor));
        BUILDER.make_instruction<jasm::Return>();
        BUILDER.leave_method();
    }

    Expression load_string_literal(context_t ctx, const Name &name)
    {
        jasm::u2 str_index = BUILDER.add_string_constant(name);
        // TODO: check if str_index <= 0xFF, otherwise use different instruction
        BUILDER.make_instruction<jasm::LoadConst>(U2_LOW(str_index));
        return Expression(TYPE_TABLE.get_class_type("java/lang/String"));
    }

    void invoke_method(context_t ctx, const Name &method_name, const ExpressionArray &arguments)
    {
        std::cout << "invoking method " << method_name << std::endl;
    }

}