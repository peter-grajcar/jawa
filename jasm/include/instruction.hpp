/**
 * @file instruction.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_INSTRUCTION_HPP
#define JAWA_INSTRUCTION_HPP

namespace jasm
{

    class Instruction
    {
    public:
        Instruction() = default;

        Instruction(Instruction &) = delete;

        Instruction(Instruction &&) = default;

        Instruction &operator=(const Instruction &) = delete;

        Instruction &operator=(Instruction &&) = default;

        virtual ~Instruction() = default;
    };

}

#endif //JAWA_INSTRUCTION_HPP
