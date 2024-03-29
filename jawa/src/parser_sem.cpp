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
#include "class.hpp"
#include <iostream>

#define BUILDER ctx->class_builder()
#define TYPE_TABLE ctx->type_table()
#define CLASS_TABLE ctx->class_table()
#define SCOPE_TABLE ctx->scope_table()
#define CLINIT ctx->static_initializer()

namespace jawa {

    void
    enter_class(context_t ctx, const Name &class_name)
    {
        std::cout << "entering class " << class_name << std::endl;
        ctx->new_class_builder(class_name);
        BUILDER.set_version(59, 0);
        BUILDER.set_access_flags(jasm::Class::ACC_PUBLIC | jasm::Class::ACC_SUPER);
    }

    void
    generate_static_initializer(context_t p_context);
    void
    leave_class(context_t ctx)
    {
        std::cout << "leaving class" << std::endl;

        // TODO: check if no constructor was created
        generate_default_constructor(ctx);

        if (CLINIT.length() > 0)
            generate_static_initializer(ctx);

        auto class_name = BUILDER.class_name();
        jasm::Class clazz = BUILDER.build();
        std::cout << clazz;

        std::ofstream os(class_name + ".class");
        clazz.emit_bytecode(os);
        os.close();
    }

    void
    generate_static_initializer(context_t ctx)
    {
        VoidTypeObs void_type = TYPE_TABLE.get_void_type();
        MethodTypeObs void_method_type = TYPE_TABLE.get_method_type(void_type, TypeObsArray());
        BUILDER.enter_method("<clinit>", *void_method_type, jasm::Method::ACC_STATIC);
        CLINIT.make_instruction<jasm::Return>();
        BUILDER.add_basic_block(std::move(CLINIT));
        BUILDER.leave_method();
    }

    bool
    is_main(context_t ctx, const Name &method_name, TypeObs return_type, TypeObsArray &argument_types)
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

    void
    enter_method(context_t ctx, const Name &method_name, TypeObs return_type, FormalParamArray &formal_params)
    {
        std::cout << "entering method " << method_name << std::endl;

        TypeObsArray argument_types;
        for (auto &formal_param : formal_params)
            argument_types.push_back(formal_param.type);

        MethodTypeObs method_type = TYPE_TABLE.get_method_type(return_type, argument_types);
        if (is_main(ctx, method_name, return_type, argument_types)) {
            BUILDER.enter_method("main", *method_type,
                                 jasm::Method::ACC_PUBLIC | jasm::Method::ACC_STATIC // <- temproary TODO: remove
            );
        } else {
            BUILDER.enter_method(method_name, *method_type);
        }

        SCOPE_TABLE.enter_scope();
        // TODO: if the function is not static
        // SCOPE_TABLE.add_var("this", TYPE_TABLE.get_class_type(BUILDER.class_name()));
        for (auto &formal_param : formal_params) {
            SCOPE_TABLE.add_var(formal_param.name, formal_param.type);
        }
    }

    void
    enter_static_initializer(context_t ctx)
    {
        std::cout << "entering static initializer" << std::endl;
        BUILDER.set_insertion_point(&CLINIT);
    }

    void
    set_method_modifiers(context_t ctx, const ModifierAndAnnotationPack &pack)
    {
        jasm::u2 flags = 0;
        if (pack.modifier_pack.get(Modifier::PUBLIC) != ModifierForm::NONE)
            flags |= jasm::Method::ACC_PUBLIC;
        if (pack.modifier_pack.get(Modifier::STATIC) != ModifierForm::NONE)
            flags |= jasm::Method::ACC_STATIC;
        if (pack.modifier_pack.get(Modifier::NATIVE) != ModifierForm::NONE)
            flags |= jasm::Method::ACC_NATIVE;
        BUILDER.current_method()->set_access_flags(flags);
    }

    void
    leave_method(context_t ctx, const ModifierAndAnnotationPack &pack)
    {
        if (BUILDER.current_method() != nullptr) {
            std::cout << "leaving method" << std::endl;
            SCOPE_TABLE.leave_scope();

            set_method_modifiers(ctx, pack);

            BUILDER.make_instruction<jasm::Return>();
            BUILDER.leave_method();
            BUILDER.set_insertion_point(&CLINIT);
        }
    }

    void
    declare_method(context_t ctx, const ModifierAndAnnotationPack &pack)
    {
        std::cout << "declaring method" << std::endl;
        set_method_modifiers(ctx, pack);
        BUILDER.leave_method();
    }

    TypeObs
    find_class(context_t ctx, const Name &name)
    {
        if (name == "Łańcuch") // temporary measure
            return TYPE_TABLE.get_class_type("java/lang/String");
        auto cls = CLASS_TABLE.load_class(name);
        assert(cls != nullptr);
        return TYPE_TABLE.get_class_type(cls->class_name());
    }

    void
    generate_default_constructor(context_t ctx)
    {
        VoidTypeObs void_type = TYPE_TABLE.get_void_type();
        MethodTypeObs void_method_type = TYPE_TABLE.get_method_type(void_type, TypeObsArray());
        BUILDER.enter_method("<init>", *void_method_type, jasm::Method::ACC_PUBLIC);
        BUILDER.make_instruction<jasm::RefLoad0>();
        jasm::u2 object_constructor = BUILDER.add_method_constant("java/lang/Object", "<init>", *void_method_type);
        BUILDER.make_instruction<jasm::InvokeSpecial>(U2_SPLIT(object_constructor));
        BUILDER.make_instruction<jasm::Return>();
        BUILDER.leave_method();
    }

    Expression
    load_string_literal(context_t ctx, const Name &name)
    {
        jasm::u2 str_index = BUILDER.add_string_constant(name);
        // TODO: check if str_index <= 0xFF, otherwise use different instruction
        BUILDER.make_instruction<jasm::LoadConst>(U2_LOW(str_index));
        return Expression(TYPE_TABLE.get_class_type("java/lang/String"));
    }

    static std::vector<std::size_t>
    split_name(const Name &name)
    {
        std::vector<std::size_t> indices;
        indices.push_back(0);
        std::size_t start = 0, end;
        while ((end = name.find('/', start)) != std::string::npos) {
            indices.push_back(end);
            start = end + 1;
        }
        indices.push_back(std::string::npos);
        return indices;
    }

    ClassAndName
    resolve_method_class(context_t ctx, const Name &method)
    {
        auto splits = split_name(method);

        if (splits.size() == 2) {
            // TODO: method in this class
            assert(false);
        }

        // TODO: check for local variables and this class fields

        Name class_name;
        std::vector<std::size_t>::iterator it;
        for (it = splits.begin() + 1; it + 1 < splits.end(); ++it) {
            Name name = CLASS_TABLE.get_fully_qualified_name(method.substr(0, *it));
            if (!name.empty()) {
                class_name = name;
                break;
            }
        }

        if (class_name.empty()) {
            ctx->message(errors::CLASS_NOT_FOUND, ctx->loc(), method.substr(0, splits[1]));
            return {};
        }

        const JawaClass *jawa_class = CLASS_TABLE.load_class(class_name);
        Name method_name = method.substr(splits[splits.size() - 2] + 1);

        if (it + 2 != splits.end()) {
            jasm::u2 field_index;
            for (; it + 2 < splits.end(); ++it) {
                // this is a bit messy
                Name field_name = method.substr(*it + 1, *(it + 1) - *it - 1);
                const JawaField *jawa_field = jawa_class->get_field(field_name);
                if (jawa_field == nullptr) {
                    ctx->message(errors::FIELD_NOT_FOUND, ctx->loc(), field_name, class_name);
                    return {};
                }
                assert(jawa_field->access_flags() & jasm::Field::ACC_STATIC);
                auto class_type = dynamic_cast<ClassTypeObs>(jawa_field->type());
                assert(class_type); // in fact it could also be an array

                field_index = BUILDER.add_field_constant(class_name, field_name,
                                                         *TYPE_TABLE.get_class_type(class_type->class_name()));
                BUILDER.make_instruction<jasm::GetStatic>(U2_SPLIT(field_index));

                jawa_class = CLASS_TABLE.load_class(class_type->class_name());
                if (jawa_class == nullptr) {
                    ctx->message(errors::CLASS_NOT_FOUND, ctx->loc(), class_type->class_name());
                    return {};
                }
                class_name = class_type->class_name();
            }

            return { class_name, method_name };
        } else {
            // static method
            return { class_name, method_name, true };
        }
    }

    Expression
    invoke_method(context_t ctx, const Expression &expr, const Name &method_name, const ExpressionArray &arguments)
    {
        TypeObsArray argument_types;
        for (auto &expr : arguments) {
            assert(expr.type != nullptr);
            argument_types.push_back(expr.type);
        }

        JawaMethodSignature signature(method_name, argument_types);

        // TODO: array_type
        auto class_type = dynamic_cast<ClassTypeObs>(expr.type);
        if (class_type == nullptr) {
            ctx->message(errors::EXPECTED_REFERENCE_TYPE, ctx->loc());
            return Expression();
        }

        const JawaClass *jawa_class = CLASS_TABLE.load_class(class_type->class_name());
        if (!jawa_class) {
            ctx->message(errors::CLASS_NOT_FOUND, ctx->loc(), class_type->class_name());
            return Expression();
        }

        const JawaMethod *jawa_method = jawa_class->get_method(signature);
        if (!jawa_method) {
            ctx->message(errors::METHOD_NOT_FOUND, ctx->loc(), method_name, class_type->class_name());
            return Expression();
        }

        jasm::u2 method_index =
          BUILDER.add_method_constant(class_type->class_name(), method_name, *jawa_method->type());
        BUILDER.make_instruction<jasm::InvokeVirtual>(U2_SPLIT(method_index));

        std::cout << "invoking method " << method_name << std::endl;
        return Expression(jawa_method->method_type()->return_type());
    }

    Expression
    invoke_method(context_t ctx, const ClassAndName &method, const ExpressionArray &arguments)
    {
        if (method.name.empty())
            return Expression();

        TypeObsArray argument_types;
        for (auto &expr : arguments) {
            assert(expr.type != nullptr);
            argument_types.push_back(expr.type);
        }

        JawaMethodSignature signature(method.name, argument_types);

        const JawaClass *jawa_class = CLASS_TABLE.load_class(method.class_name);
        if (!jawa_class) {
            ctx->message(errors::CLASS_NOT_FOUND, ctx->loc(), method.class_name);
            return Expression();
        }

        const JawaMethod *jawa_method = jawa_class->get_method(signature);
        if (!jawa_method) {
            ctx->message(errors::METHOD_NOT_FOUND, ctx->loc(), method.name, method.class_name);
            return Expression();
        }

        jasm::u2 method_index = BUILDER.add_method_constant(method.class_name, method.name, *jawa_method->type());

        if (method.is_static) {
            BUILDER.make_instruction<jasm::InvokeStatic>(U2_SPLIT(method_index));
        } else {
            BUILDER.make_instruction<jasm::InvokeVirtual>(U2_SPLIT(method_index));
        }

        std::cout << "invoking method " << method.name << std::endl;
        return Expression(jawa_method->method_type()->return_type());
    }

    Expression
    load_name(context_t ctx, const Name &name)
    {
        auto *var = SCOPE_TABLE.get_var(name);
        if (var == nullptr) {
            ctx->message(errors::VARIABLE_NOT_DECLARED, ctx->loc(), name);
            return Expression{};
        }
        std::cout << "name expression " << name << std::endl;
        // TODO: check if the variable has a reference type
        switch (var->index) {
        case 0:
            BUILDER.make_instruction<jasm::RefLoad0>();
            break;
        case 1:
            BUILDER.make_instruction<jasm::RefLoad1>();
            break;
        case 2:
            BUILDER.make_instruction<jasm::RefLoad2>();
            break;
        case 3:
            BUILDER.make_instruction<jasm::RefLoad3>();
            break;
        default:
            BUILDER.make_instruction<jasm::RefLoad>(U2_LOW(var->index));
            break;
        }
        return Expression(var->type);
    }

    void
    declare_field(context_t ctx, const ModifierAndAnnotationPack &pack, TypeObs type, const Name &name)
    {
        std::cout << "declaring field " << name << std::endl;
        BUILDER.declare_field(name, *type,
                              jasm::Field::ACC_PUBLIC | jasm::Field::ACC_STATIC); // <- temporary solution TODO: remove
    }

    Expression
    instantiate_object(context_t ctx, const Name &class_name)
    {
        std::cout << "instantiate new class " << class_name << std::endl;
        auto cls = CLASS_TABLE.load_class(class_name);
        assert(cls != nullptr);

        ClassTypeObs type = TYPE_TABLE.get_class_type(cls->class_name());
        jasm::u2 class_idx = BUILDER.add_class_constant(cls->class_name());
        BUILDER.make_instruction<jasm::New>(U2_SPLIT(class_idx));
        BUILDER.make_instruction<jasm::Duplicate>();

        VoidTypeObs void_type = TYPE_TABLE.get_void_type();
        // TODO: support for different types of constructors
        MethodTypeObs constructor_type = TYPE_TABLE.get_method_type(void_type, TypeObsArray());
        jasm::u2 constructor_idx = BUILDER.add_method_constant(cls->class_name(), "<init>", *constructor_type);
        BUILDER.make_instruction<jasm::InvokeSpecial>(U2_SPLIT(constructor_idx));

        return Expression(type);
    }

    Expression
    assign(context_t ctx, const Name &name, const Expression &expr)
    {
        // TODO: this is an ad hoc function in its entirety
        std::cout << "assigning to " << name << std::endl;
        jasm::u2 field_idx = BUILDER.add_field_constant(BUILDER.class_name(), name, *expr.type);
        BUILDER.make_instruction<jasm::PutStatic>(U2_SPLIT(field_idx));
        return expr;
    }

    void
    set_package_name(context_t ctx, const Name &name)
    {
        ctx->set_package_name(name);
    }

    void
    import(context_t ctx, const Name &name)
    {
        std::cout << "importing " << name << std::endl;
        CLASS_TABLE.import_class(name);
    }

}