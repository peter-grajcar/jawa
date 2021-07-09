/**
 * @file builder_test.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "class.hpp"
#include "builder.hpp"

#define U2_HIGH(X) ((u1) ((X & 0xFF00u) >> 8u))
#define U2_LOW(X) ((u1) (X & 0x00FFu))

using namespace jasm;

int main()
{
    Class clazz(0, 59, Class::AccessFlag::ACC_PUBLIC);

    auto &pool = clazz.constant_pool();

    u2 class_name = pool.make_constant<Utf8Constant>("HelloWorld");
    u2 main = pool.make_constant<Utf8Constant>("main");
    u2 main_signature = pool.make_constant<Utf8Constant>("([Ljava/lang/String;)V");
    u2 object = pool.make_constant<Utf8Constant>("java/lang/Object");
    u2 constructor = pool.make_constant<Utf8Constant>("<init>");
    u2 constructor_signature = pool.make_constant<Utf8Constant>("()V");
    u2 system = pool.make_constant<Utf8Constant>("java/lang/System");
    u2 out = pool.make_constant<Utf8Constant>("out");
    u2 println = pool.make_constant<Utf8Constant>("println");
    u2 println_signature = pool.make_constant<Utf8Constant>("(Ljava/lang/String;)V");
    u2 print_stream = pool.make_constant<Utf8Constant>("java/io/PrintStream");
    u2 print_stream_type = pool.make_constant<Utf8Constant>("Ljava/io/PrintStream;");
    u2 str_val = pool.make_constant<Utf8Constant>("Hello, World!");
    u2 str = pool.make_constant<StringConstant>(str_val);
    u2 code = pool.make_constant<Utf8Constant>("Code");

    u2 test_class = pool.make_constant<ClassConstant>(class_name);
    u2 object_class = pool.make_constant<ClassConstant>(object);
    u2 system_class = pool.make_constant<ClassConstant>(system);
    u2 print_stream_class = pool.make_constant<ClassConstant>(print_stream);

    u2 println_name_and_type = pool.make_constant<NameAndTypeConstant>(println, println_signature);
    u2 constructor_name_and_type = pool.make_constant<NameAndTypeConstant>(constructor,
                                                                           constructor_signature);
    u2 out_name_and_type = pool.make_constant<NameAndTypeConstant>(out, print_stream_type);

    u2 object_constructor_method = pool.make_constant<MethodRefConstant>(object_class, constructor_name_and_type);
    u2 println_method = pool.make_constant<MethodRefConstant>(print_stream_class, println_name_and_type);
    u2 out_field = pool.make_constant<FieldRefConstant>(system_class, out_name_and_type);

    Method test_constructor(Method::ACC_PUBLIC, constructor, constructor_signature);
    CodeAttribute test_constructor_code(code, 1, 1);
    test_constructor_code.make_instruction<RefLoad0>();
    test_constructor_code.make_instruction<InvokeSpecial>(U2_HIGH(object_constructor_method),
                                                          U2_LOW(object_constructor_method));
    test_constructor_code.make_instruction<Return>();
    test_constructor.add_attribute(std::move(test_constructor_code));

    Method hello_world_main(Method::ACC_PUBLIC | Method::ACC_STATIC, main, main_signature);
    CodeAttribute hello_world_main_code(code, 2, 1);
    hello_world_main_code.make_instruction<GetStatic>(U2_HIGH(out_field), U2_LOW(out_field));
    hello_world_main_code.make_instruction<LoadConst>(U2_LOW(str));
    hello_world_main_code.make_instruction<InvokeVirtual>(U2_HIGH(println_method),
                                                          U2_LOW(println_method));
    hello_world_main_code.make_instruction<Return>();
    hello_world_main.add_attribute(std::move(hello_world_main_code));

    clazz.set_this_class(test_class);
    clazz.set_super_class(object_class);
    clazz.add_method(std::move(test_constructor));
    clazz.add_method(std::move(hello_world_main));

    std::cout << clazz;

    std::ofstream os("jasm/test/classes/HelloWorld.class");
    clazz.emit_bytecode(os);
    os.close();

    return 0;
}
