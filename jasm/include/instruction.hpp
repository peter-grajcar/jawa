/**
 * @file instruction.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_INSTRUCTION_HPP
#define JAWA_INSTRUCTION_HPP

#include "byte_code.hpp"
#include "mnemonics.hpp"

namespace jasm
{

    using namespace byte_code;

    class Instruction
    {
    public:
        Instruction() = default;

        Instruction(Instruction &) = delete;

        Instruction(Instruction &&) = default;

        Instruction &operator=(const Instruction &) = delete;

        Instruction &operator=(Instruction &&) = default;

        virtual ~Instruction() = default;

        virtual u1 opcode() const = 0;

        virtual const char *mnemonic() const = 0;

        virtual u2 operand_count() const = 0;

        virtual u2 input_stack_operand_count() const = 0;

        virtual u2 output_stack_operand_count() const = 0;
    };

    template <u1 opcode_, u2 operand_count_, u2 stack_in_, u2 stack_out_>
    class JVMInstruction : Instruction
    {
        std::array<u1, operand_count_> operands_;
    public:
        template <typename ...Args>
        explicit JVMInstruction(Args ...args) : operands_{args...} {};

        inline u1 opcode() const
        {
            return opcode_;
        }

        inline const char *mnemonic() const
        {
            return InstructionMnemonics[opcode_];
        }

        inline u2 operand_count() const
        {
            return operand_count_;
        }

        inline u2 input_stack_operand_count() const
        {
            return stack_in_;
        }

        inline u2 output_stack_operand_count() const
        {
            return stack_out_;
        }

    };

    using AALoad = JVMInstruction<0x32, 0, 2, 1>;
    using AAStore = JVMInstruction<0x53, 0, 3, 0>;
    using AConstNull = JVMInstruction<0x1, 0, 0, 1>;
    using ALoad = JVMInstruction<0x19, 1, 0, 1>;
    using ALoad0 = JVMInstruction<0x2a, 0, 0, 1>;
    using ALoad1 = JVMInstruction<0x2b, 0, 0, 1>;
    using ALoad2 = JVMInstruction<0x2c, 0, 0, 1>;
    using ALoad3 = JVMInstruction<0x2d, 0, 0, 1>;
    using ANewArray = JVMInstruction<0xbd, 2, 1, 1>;
    using AReturn = JVMInstruction<0xb0, 0, 1, 0>;


}

#endif //JAWA_INSTRUCTION_HPP
