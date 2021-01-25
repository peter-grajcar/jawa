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
        virtual ~Instruction() = default;
    };

}

#endif //JAWA_INSTRUCTION_HPP
