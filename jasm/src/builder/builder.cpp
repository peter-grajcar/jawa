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
            : class_(), current_insertion_point_(), basic_blocks_(), utf8_constants_()
    {
        u2 class_name_const = class_.constant_pool_.make_constant<Utf8Constant>(class_name);
        class_.this_class_ = class_.constant_pool_.make_constant<ClassConstant>(class_name_const);
        utf8_constants_.insert({class_name, class_name_const});
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

    ClassBuilder &ClassBuilder::add_access_flags(u2 access_flags)
    {
        class_.access_flags_ |= access_flags;
        return *this;
    }

}
