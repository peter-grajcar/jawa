/**
 * @file builder_test.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include "class.hpp"
#include "builder/builder.hpp"

#define U2_HIGH(X) ((u1) ((X & 0xFF00u) >> 8u))
#define U2_LOW(X) ((u1) (X & 0x00FFu))

using namespace jasm;

int main()
{
    ClassBuilder builder("HelloWorld");
    builder.set_version(59, 0)
            .set_access_flags(Class::ACC_PUBLIC);

    ClassType str_type("java/lang/String");
    ClassType print_stream("java/io/PrintStream");
    ArrayType str_arr(&str_type, 1);
    PrimitiveType void_type = VoidType();
    MethodSignatureType main_signature(&void_type, &str_arr);
    MethodSignatureType constructor_signature(&void_type);
    MethodSignatureType println_signature(&void_type, &str_type);

    u2 object_constructor = builder.add_method_constant("java/lang/Object", "<init>", constructor_signature);

    builder.enter_constructor(constructor_signature, Method::ACC_PUBLIC);
    builder.make_instruction<RefLoad0>();
    builder.make_instruction<InvokeSpecial>(U2_HIGH(object_constructor),
                                            U2_LOW(object_constructor));
    builder.make_instruction<Return>();
    builder.exit_method();

    u2 message = builder.add_string_constant("Hello, World!");
    u2 out_field = builder.add_field_constant("java/lang/System", "out", print_stream);
    u2 println_method = builder.add_method_constant("java/io/PrintStream", "println", println_signature);

    builder.enter_method("main", main_signature, Method::ACC_PUBLIC | Method::ACC_STATIC);
    builder.make_instruction<GetStatic>(U2_HIGH(out_field), U2_LOW(out_field));
    builder.make_instruction<LoadConst>(U2_LOW(message));
    builder.make_instruction<InvokeVirtual>(U2_HIGH(println_method), U2_LOW(println_method));
    builder.make_instruction<Return>();
    builder.exit_method();

    Class clazz = builder.build();
    std::cout << clazz;

    std::ofstream os("jasm/test/classes/HelloWorld.class");
    clazz.emit_bytecode(os);
    os.close();

    return 0;
}
