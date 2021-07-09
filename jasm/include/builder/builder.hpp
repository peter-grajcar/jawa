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

#include <vector>
#include <map>
#include <string>
#include <class.hpp>

#include "instruction.hpp"
#include "class.hpp"

namespace jasm
{

    class BasicBlock
    {
    private:
        std::vector<std::unique_ptr<Instruction>> code_;
    public:

        template <typename T, typename ...Args>
        inline void make_instruction(Args ...args)
        {
            code_.emplace_back(std::make_unique<T>(args...));
        }

        u4 length() const;

    };

    class ClassBuilder
    {
    public:
        using InsertionPoint = BasicBlock *;
    private:
        std::vector<BasicBlock> basic_blocks_;
        InsertionPoint current_insertion_point_;

        std::map<utf8, u2> utf8_constants_;

        Class class_;

    public:
        ClassBuilder(utf8 &class_name);

        InsertionPoint create_basic_block();

        ClassBuilder &set_version(u2 major_version, u2 minor_version);

        ClassBuilder &set_access_flags(u2 access_flags);

        ClassBuilder &add_access_flags(u2 access_flags);

        inline Class build()
        {
            return std::move(class_);
        }
    };

}

#endif //JAWA_BUILDER_HPP
