/**
 * @file builder.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) 2021 Peter Grajcar
 */

#include <builder/builder.hpp>

namespace jasm
{

    u4 jasm::BasicBlock::length() const
    {
        u4 length = 0;
        for (auto &inst : code_)
            length += inst->size();
        return length;
    }

    ClassBuilder::ClassBuilder(utf8 &class_name)
            : current_insertion_point_(), class_name_(class_name), current_code_(), current_method_()
    {
        init();
    }

    ClassBuilder::ClassBuilder(const char *class_name)
            : current_insertion_point_(), class_name_(class_name), current_code_(), current_method_()
    {
        init();
    }

    void ClassBuilder::init()
    {
        class_.this_class_ = add_class_constant(class_name_);
        utf8 object = "java/lang/Object";
        class_.super_class_ = add_class_constant(object);
    }


    ClassBuilder::InsertionPoint ClassBuilder::create_basic_block()
    {
        basic_blocks_.emplace_back();
        return &basic_blocks_.back();
    }

    ClassBuilder &ClassBuilder::set_version(u2 major_version, u2 minor_version)
    {
        class_.minor_version_ = minor_version;
        class_.major_version_ = major_version;
        return *this;
    }

    ClassBuilder &ClassBuilder::set_access_flags(u2 access_flags)
    {
        class_.access_flags_ = access_flags;
        return *this;
    }

    ClassBuilder &ClassBuilder::add_access_flags(Class::AccessFlag access_flag)
    {
        class_.access_flags_ |= access_flag;
        return *this;
    }

    void ClassBuilder::set_insertion_point(ClassBuilder::InsertionPoint insertion_point)
    {
        current_insertion_point_ = insertion_point;
    }

    u2 ClassBuilder::add_utf8_constant(const utf8 &value)
    {
        auto search = utf8_constants_.find(value);
        if (search != utf8_constants_.end())
            return search->second;
        u2 index = class_.constant_pool_.make_constant<Utf8Constant>(value);
        utf8_constants_.insert({value, index});
        return index;
    }

    u2 ClassBuilder::add_class_constant(const utf8 &class_name)
    {
        auto search = class_constants_.find(class_name);
        if (search != class_constants_.end())
            return search->second;
        u2 name_index = add_utf8_constant(class_name);
        return class_.constant_pool_.make_constant<ClassConstant>(name_index);
    }

    u2 ClassBuilder::add_name_and_type_constant(const utf8 &name, const Type &type)
    {
        u2 name_index = add_utf8_constant(name);
        utf8 type_descriptor = type.descriptor();
        u2 type_index = add_utf8_constant(type_descriptor);
        return class_.constant_pool_.make_constant<NameAndTypeConstant>(name_index, type_index);
    }

    u2 ClassBuilder::add_method_constant(const utf8 &class_name, const utf8 &method_name, const Type &type)
    {
        u2 name_and_type_index = add_name_and_type_constant(method_name, type);
        u2 class_index = add_class_constant(class_name_);
        return class_.constant_pool_.make_constant<MethodRefConstant>(class_index, name_and_type_index);
    }

    u2 ClassBuilder::add_field_constant(const utf8 &class_name, const utf8 &field_name, const Type &type)
    {
        u2 name_and_type_index = add_name_and_type_constant(field_name, type);
        u2 class_index = add_class_constant(class_name_);
        return class_.constant_pool_.make_constant<FieldRefConstant>(class_index, name_and_type_index);
    }

    ClassBuilder::InsertionPoint ClassBuilder::enter_constructor(const Type &type, u2 access_flags)
    {
        return enter_method("<init>", type, access_flags);
    }

    ClassBuilder::InsertionPoint ClassBuilder::enter_method(const utf8 &method_name, const Type &type, u2 access_flags)
    {
        InsertionPoint insertion_point = create_basic_block();
        current_insertion_point_ = insertion_point;

        u2 name_index = add_utf8_constant(method_name);
        u2 descriptor_index = add_utf8_constant(type.descriptor());
        Method method(access_flags, name_index, descriptor_index);
        class_.add_method(std::move(method));
        current_method_ = &class_.methods_.back();

        u2 code_attr_name = add_utf8_constant("Code");
        current_method_->make_attribute<CodeAttribute>(code_attr_name, 0, 0);
        current_code_ = dynamic_cast<CodeAttribute *>(current_method_->attributes().back().get());

        return insertion_point;
    }

    void ClassBuilder::exit_method()
    {
        for (auto &basic_block : basic_blocks_) {
            for (auto &inst : basic_block.code_) {
                current_code_->add_instruction(std::move(inst));
            }
        }
        // TODO:
    }

}
