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

        virtual void jasm(std::ostream &os) const = 0;
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

        inline void jasm(std::ostream &os) const override
        {
            os << mnemonic() << ' ';
            for (u1 op : operands_)
                os << std::hex << op << ' ';
            os << std::endl;
        }
    };

    using RefArrayLoad = JVMInstruction<0x32, 0, 2, 1>;
    using RefArrayStore = JVMInstruction<0x53, 0, 3, 0>;
    using RefConstNull = JVMInstruction<0x1, 0, 0, 1>;
    using RefLoad = JVMInstruction<0x19, 1, 0, 1>;
    using RefLoad0 = JVMInstruction<0x2a, 0, 0, 1>;
    using RefLoad1 = JVMInstruction<0x2b, 0, 0, 1>;
    using RefLoad2 = JVMInstruction<0x2c, 0, 0, 1>;
    using RefLoad3 = JVMInstruction<0x2d, 0, 0, 1>;
    using RefNewArray = JVMInstruction<0xbd, 2, 1, 1>;
    using RefReturn = JVMInstruction<0xb0, 0, 1, 0>;
    using ArrayLength = JVMInstruction<0xbe, 0, 1, 1>;
    using RefStore = JVMInstruction<0x3a, 1, 1, 0>;
    using RefStore0 = JVMInstruction<0x4b, 0, 1, 0>;
    using RefStore1 = JVMInstruction<0x4c, 0, 1, 0>;
    using RefStore2 = JVMInstruction<0x4d, 0, 1, 0>;
    using RefStore3 = JVMInstruction<0x4e, 0, 1, 0>;
    using RefThrow = JVMInstruction<0xbf, 0, 1, 1>;
    using ByteArrayLoad = JVMInstruction<0x33, 0, 2, 1>;
    using ByteArrayStore = JVMInstruction<0x54, 0, 3, 0>;
    using BytePush = JVMInstruction<0x10, 1, 0, 1>;
    using CharArrayLoad = JVMInstruction<0x34, 0, 2, 1>;
    using CharArrayStore = JVMInstruction<0x55, 0, 3, 0>;
    using CheckCast = JVMInstruction<0xc0, 2, 1, 1>;
    using DoubleToFloat = JVMInstruction<0x90, 0, 1, 1>;
    using DoubleToInt = JVMInstruction<0x82, 0, 1, 1>;
    using DoubleToLong = JVMInstruction<0x8f, 0, 1, 1>;
    using DoubleAdd = JVMInstruction<0x63, 0, 2, 1>;
    using DoubleArrayLoad = JVMInstruction<0x31, 0, 2, 1>;
    using DoubleArrayStore = JVMInstruction<0x52, 0, 3, 0>;
    using DoubleCmpL = JVMInstruction<0x97, 0, 2, 1>;
    using DoubleCmpG = JVMInstruction<0x98, 0, 2, 1>;
    using DoubleConst0 = JVMInstruction<0xe, 0, 0, 1>;
    using DoubleConst1 = JVMInstruction<0xf, 0, 0, 1>;
    using DoubleDiv = JVMInstruction<0x6f, 0, 2, 1>;
    using DoubleLoad = JVMInstruction<0x18, 1, 0, 1>;
    using DoubleLoad0 = JVMInstruction<0x26, 0, 0, 1>;
    using DoubleLoad1 = JVMInstruction<0x27, 0, 0, 1>;
    using DoubleLoad2 = JVMInstruction<0x28, 0, 0, 1>;
    using DoubleLoad3 = JVMInstruction<0x29, 0, 0, 1>;
    using DoubleMul = JVMInstruction<0x6b, 0, 2, 1>;
    using DoubleNeg = JVMInstruction<0x77, 0, 1, 1>;
    using DoubleRem = JVMInstruction<0x73, 0, 2, 1>;
    using DoubleReturn = JVMInstruction<0xaf, 0, 1, 0>;
    using DoubleStore = JVMInstruction<0x39, 1, 1, 0>;
    using DoubleStore0 = JVMInstruction<0x47, 0, 1, 0>;
    using DoubleStore1 = JVMInstruction<0x48, 0, 1, 0>;
    using DoubleStore2 = JVMInstruction<0x49, 0, 1, 0>;
    using DoubleStore3 = JVMInstruction<0x4a, 0, 1, 0>;
    using DoubleSub = JVMInstruction<0x67, 0, 2, 1>;
    using Duplicate = JVMInstruction<0x59, 0, 1, 2>;
    using DuplicateX1 = JVMInstruction<0x5a, 0, 2, 3>;
    using DuplicateX2 = JVMInstruction<0x5b, 0, 3, 4>;
    using Duplicate2 = JVMInstruction<0x5c, 0, 2, 4>;
    using Duplicate2X1 = JVMInstruction<0x5d, 0, 3, 5>;
    using Duplicate2X2 = JVMInstruction<0x5e, 0, 4, 6>;
    using FloatToDouble = JVMInstruction<0x8d, 0, 1, 1>;
    using FloatToInt = JVMInstruction<0x8b, 0, 1, 1>;
    using FloatToLong = JVMInstruction<0x8c, 0, 1, 1>;
    using FloatAdd = JVMInstruction<0x62, 0, 2, 1>;
    using FloatArrayLoad = JVMInstruction<0x30, 0, 2, 1>;
    using FloatArrayStore = JVMInstruction<0x51, 0, 3, 0>;
    using FloatCmpL = JVMInstruction<0x95, 0, 2, 1>;
    using FloatCmpG = JVMInstruction<0x96, 0, 2, 1>;
    using FloatConst0 = JVMInstruction<0xb, 0, 0, 1>;
    using FloatConst1 = JVMInstruction<0xc, 0, 0, 1>;
    using FloatConst2 = JVMInstruction<0xd, 0, 0, 1>;
    using FloatDiv = JVMInstruction<0x6e, 0, 2, 1>;
    using FloatLoad = JVMInstruction<0x96, 1, 0, 1>;
    using FloatLoad0 = JVMInstruction<0x22, 0, 0, 1>;
    using FloatLoad1 = JVMInstruction<0x23, 0, 0, 1>;
    using FloatLoad2 = JVMInstruction<0x24, 0, 0, 1>;
    using FloatLoad3 = JVMInstruction<0x25, 0, 0, 1>;
    using FloatMul = JVMInstruction<0x6a, 0, 2, 1>;
    using FloatNeg = JVMInstruction<0x76, 0, 1, 1>;
    using FloatRen = JVMInstruction<0x72, 0, 2, 1>;
    using FloatReturn = JVMInstruction<0xae, 0, 1, 0>;
    using FloatStore = JVMInstruction<0x38, 1, 1, 0>;
    using FloatStore0 = JVMInstruction<0x43, 0, 1, 0>;
    using FloatStore1 = JVMInstruction<0x44, 0, 1, 0>;
    using FloatStore2 = JVMInstruction<0x45, 0, 1, 0>;
    using FloatStore3 = JVMInstruction<0x46, 0, 1, 0>;
    using FloatSub = JVMInstruction<0x66, 0, 2, 1>;
    using GetField = JVMInstruction<0xb4, 2, 1, 1>;
    using GetStatic = JVMInstruction<0xb2, 2, 0, 1>;
    using GoTo = JVMInstruction<0xa7, 2, 0, 0>;
    using GoToW = JVMInstruction<0xc8, 4, 0, 0>;
    using IntToByte = JVMInstruction<0x91, 0, 1, 1>;
    using IntToChar = JVMInstruction<0x92, 0, 1, 1>;
    using IntToDouble = JVMInstruction<0x87, 0, 1, 1>;
    using IntToFloat = JVMInstruction<0x86, 0, 1, 1>;
    using IntToLong = JVMInstruction<0x85, 0, 1, 1>;
    using IntToShort = JVMInstruction<0x93, 0, 1, 1>;
    using IntAdd = JVMInstruction<0x60, 0, 2, 1>;
    using IntArrayLoad = JVMInstruction<0x2e, 0, 2, 1>;
    using IntAnd = JVMInstruction<0x7e, 0, 2, 1>;
    using IntArrayStore = JVMInstruction<0x4f, 0, 3, 0>;
    using IntConstNeg1 = JVMInstruction<0x2, 0, 0, 1>;
    using IntConst0 = JVMInstruction<0x3, 0, 0, 1>;
    using IntConst1 = JVMInstruction<0x4, 0, 0, 1>;
    using IntConst2 = JVMInstruction<0x5, 0, 0, 1>;
    using IntConst3 = JVMInstruction<0x6, 0, 0, 1>;
    using IntConst4 = JVMInstruction<0x7, 0, 0, 1>;
    using IntConst5 = JVMInstruction<0x8, 0, 0, 1>;
    using IntDiv = JVMInstruction<0x6c, 0, 2, 1>;
    using IfRefCmpEq = JVMInstruction<0xa5, 2, 2, 0>;
    using IfRefCmpNe = JVMInstruction<0xa6, 2, 2, 0>;
    using IfIntCmpEq = JVMInstruction<0x9f, 2, 2, 0>;
    using IfIntCmpNe = JVMInstruction<0xa0, 2, 2, 0>;
    using IfIntCmpLt = JVMInstruction<0xa1, 2, 2, 0>;
    using IfIntCmpGe = JVMInstruction<0xa2, 2, 2, 0>;
    using IfIntCmpGt = JVMInstruction<0xa3, 2, 2, 0>;
    using IfIntCmpLe = JVMInstruction<0xa4, 2, 2, 0>;
    using IfEq = JVMInstruction<0x99, 2, 1, 0>;
    using IfNe = JVMInstruction<0x9a, 2, 1, 0>;
    using IfLt = JVMInstruction<0x9b, 2, 1, 0>;
    using IfGe = JVMInstruction<0x9c, 2, 1, 0>;
    using IfGt = JVMInstruction<0x9d, 2, 1, 0>;
    using IfLe = JVMInstruction<0x9e, 2, 1, 0>;
    using IfNonNull = JVMInstruction<0xc7, 2, 1, 0>;
    using IfNull = JVMInstruction<0xc6, 2, 1, 0>;
    using IntInc = JVMInstruction<0x84, 2, 0, 0>;
    using IntLoad = JVMInstruction<0x15, 1, 0, 1>;
    using IntLoad0 = JVMInstruction<0x1a, 0, 0, 1>;
    using IntLoad1 = JVMInstruction<0x1b, 0, 0, 1>;
    using IntLoad2 = JVMInstruction<0x1c, 0, 0, 1>;
    using IntLoad3 = JVMInstruction<0x1d, 0, 0, 1>;


}

#endif //JAWA_INSTRUCTION_HPP
