/**
 * @file builder.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_BUILDER_HPP
#define JAWA_BUILDER_HPP

#include <class.hpp>
#include <map>
#include <string>
#include <vector>

#include "class.hpp"
#include "instruction.hpp"
#include "type.hpp"

namespace jasm {

    class BasicBlock
    {
    private:
        std::vector<std::unique_ptr<Instruction>> code_;

        friend class ClassBuilder;

    public:
        template<typename T, typename... Args>
        inline void
        make_instruction(Args... args)
        {
            code_.emplace_back(std::make_unique<T>(args...));
        }

        u4
        length() const;
    };

    class ClassBuilder
    {
    public:
        using InsertionPoint = BasicBlock *;

    private:
        std::vector<BasicBlock> basic_blocks_;
        InsertionPoint current_insertion_point_;
        CodeAttribute *current_code_;
        Method *current_method_;

        std::map<utf8, u2> utf8_constants_;
        std::map<utf8, u2> string_constants_;
        std::map<utf8, u2> class_constants_;

        utf8 class_name_;
        Class class_;

        void
        init();

    public:
        ClassBuilder(utf8 class_name);

        explicit ClassBuilder(const char *class_name);

        InsertionPoint
        create_basic_block();

        InsertionPoint
        enter_method(const utf8 &method_name, const MethodType &type, u2 access_flags = 0);

        void
        leave_method();

        void
        set_insertion_point(InsertionPoint insertion_point);

        ClassBuilder &
        set_version(u2 major_version, u2 minor_version);

        ClassBuilder &
        set_access_flags(u2 access_flags);

        u2
        add_utf8_constant(const utf8 &value);

        u2
        add_class_constant(const utf8 &class_name);

        u2
        add_name_and_type_constant(const utf8 &name, const Type &type);

        u2
        add_method_constant(const utf8 &class_name, const utf8 &method_name, const Type &type);

        u2
        add_field_constant(const utf8 &class_name, const utf8 &field_name, const Type &type);

        u2
        add_string_constant(const utf8 &str);

        inline Method *
        current_method()
        {
            return current_method_;
        }

        template<typename T, typename... Args>
        inline void
        make_instruction(Args... args)
        {
            assert(current_insertion_point_);
            current_insertion_point_->make_instruction<T>(args...);
        }

        inline Class
        build()
        {
            return std::move(class_);
        }

        inline utf8
        class_name() const
        {
            return class_name_;
        }
    };

}

#endif // JAWA_BUILDER_HPP
