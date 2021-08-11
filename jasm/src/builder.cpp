/**
 * @file builder.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#include <builder.hpp>
#include <utility>

namespace jasm {

    u4
    jasm::BasicBlock::length() const
    {
        u4 length = 0;
        for (auto &inst : code_)
            length += inst->size();
        return length;
    }

    ClassBuilder::ClassBuilder(utf8 class_name)
      : current_insertion_point_()
      , current_code_()
      , current_method_()
      , class_name_(std::move(class_name))
    {
        init();
    }

    ClassBuilder::ClassBuilder(const char *class_name)
      : current_insertion_point_()
      , current_code_()
      , current_method_()
      , class_name_(class_name)
    {
        init();
    }

    void
    ClassBuilder::init()
    {
        class_.this_class_ = add_class_constant(class_name_);
        utf8 object = "java/lang/Object";
        class_.super_class_ = add_class_constant(object);
    }

    ClassBuilder::InsertionPoint
    ClassBuilder::create_basic_block()
    {
        basic_blocks_.emplace_back();
        return &basic_blocks_.back();
    }

    void
    ClassBuilder::add_basic_block(BasicBlock &&basic_block)
    {
        basic_blocks_.emplace_back(std::forward<BasicBlock &&>(basic_block));
    }

    ClassBuilder &
    ClassBuilder::set_version(u2 major_version, u2 minor_version)
    {
        class_.minor_version_ = minor_version;
        class_.major_version_ = major_version;
        return *this;
    }

    ClassBuilder &
    ClassBuilder::set_access_flags(u2 access_flags)
    {
        class_.access_flags_ = access_flags;
        return *this;
    }

    void
    ClassBuilder::set_insertion_point(ClassBuilder::InsertionPoint insertion_point)
    {
        current_insertion_point_ = insertion_point;
    }

    u2
    ClassBuilder::add_utf8_constant(const utf8 &value)
    {
        auto search = utf8_constants_.find(value);
        if (search != utf8_constants_.end())
            return search->second;
        u2 index = class_.constant_pool_.make_constant<Utf8Constant>(value);
        utf8_constants_.insert({ value, index });
        return index;
    }

    u2
    ClassBuilder::add_class_constant(const utf8 &class_name)
    {
        auto search = class_constants_.find(class_name);
        if (search != class_constants_.end())
            return search->second;
        u2 name_index = add_utf8_constant(class_name);
        return class_.constant_pool_.make_constant<ClassConstant>(name_index);
    }

    u2
    ClassBuilder::add_name_and_type_constant(const utf8 &name, const Type &type)
    {
        u2 name_index = add_utf8_constant(name);
        utf8 type_descriptor = type.descriptor();
        u2 type_index = add_utf8_constant(type_descriptor);
        return class_.constant_pool_.make_constant<NameAndTypeConstant>(name_index, type_index);
    }

    u2
    ClassBuilder::add_method_constant(const utf8 &class_name, const utf8 &method_name, const Type &type)
    {
        u2 name_and_type_index = add_name_and_type_constant(method_name, type);
        u2 class_index = add_class_constant(class_name);
        return class_.constant_pool_.make_constant<MethodRefConstant>(class_index, name_and_type_index);
    }

    u2
    ClassBuilder::add_field_constant(const utf8 &class_name, const utf8 &field_name, const Type &type)
    {
        u2 name_and_type_index = add_name_and_type_constant(field_name, type);
        u2 class_index = add_class_constant(class_name);
        return class_.constant_pool_.make_constant<FieldRefConstant>(class_index, name_and_type_index);
    }

    u2
    ClassBuilder::add_string_constant(const utf8 &str)
    {
        auto search = string_constants_.find(str);
        if (search != string_constants_.end())
            return search->second;
        u2 utf8_index = class_.constant_pool_.make_constant<Utf8Constant>(str);
        u2 index = class_.constant_pool_.make_constant<StringConstant>(utf8_index);
        string_constants_.insert({ str, index });
        return index;
    }

    ClassBuilder::InsertionPoint
    ClassBuilder::enter_method(const utf8 &method_name, const MethodType &type, u2 access_flags)
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

        current_code_->set_locals_limit(1 + type.argument_types().size());

        return insertion_point;
    }

    void
    ClassBuilder::leave_method()
    {
        if (current_method_->access_flags() & Method::ACC_NATIVE) {
            current_method_->attributes().erase(std::remove_if(
              current_method_->attributes().begin(), current_method_->attributes().end(),
              [](std::unique_ptr<Attribute> &attr) { return dynamic_cast<CodeAttribute *>(attr.get()) != nullptr; }));
        }
        u2 stack_size = 0;
        u2 max_stack_size = 0;
        for (auto &basic_block : basic_blocks_) {
            for (auto &inst : basic_block.code_) {
                stack_size -= inst->input_stack_operand_count();
                stack_size += inst->output_stack_operand_count();

                if (stack_size > max_stack_size)
                    max_stack_size = stack_size;

                current_code_->add_instruction(std::move(inst));
            }
        }
        current_code_->set_stack_limit(max_stack_size);
        basic_blocks_.clear();
    }

    void
    ClassBuilder::declare_field(const utf8 &field_name, const Type &type, u2 access_flags)
    {
        u2 name_index = add_utf8_constant(field_name);
        u2 descriptor_index = add_utf8_constant(type.descriptor());
        Field field(access_flags, name_index, descriptor_index);
        class_.add_field(std::move(field));
    }

}
