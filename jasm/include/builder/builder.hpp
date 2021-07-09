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

#include "instruction.hpp"

namespace jasm
{

    class InsertionPoint
    {
    private:
        std::vector<std::unique_ptr<Instruction>> code_;
    public:

        template <typename T, typename ...Args>
        inline void make_instruction(Args ...args)
        {
            code_.emplace_back(std::make_unique<T>(args...));
        }

    };

    class ClassBuilder
    {
    private:
        std::vector<InsertionPoint> insertion_points_;
        std::vector<InsertionPoint>::iterator current_insertion_point_;

    public:
        ClassBuilder() = default;
    };

}

#endif //JAWA_BUILDER_HPP
