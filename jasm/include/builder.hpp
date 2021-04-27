/**
 * @file builder.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_BUILDER_HPP
#define JAWA_BUILDER_HPP

#include <vector>
#include <list>

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

    public:
        ClassBuilder() = default;
    };

}

#endif //JAWA_BUILDER_HPP
