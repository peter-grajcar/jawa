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
    class SimpleInstruction : Instruction
    {
        std::array<u1, operand_count_> operands_;
    public:
        template <typename ...Args>
        explicit SimpleInstruction(Args ...args) : operands_{args...} {};

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

    using RefArrayLoad = SimpleInstruction<0x32, 0, 2, 1>;
    using RefArrayStore = SimpleInstruction<0x53, 0, 3, 0>;
    using RefConstNull = SimpleInstruction<0x1, 0, 0, 1>;
    using RefLoad = SimpleInstruction<0x19, 1, 0, 1>;
    using RefLoad0 = SimpleInstruction<0x2a, 0, 0, 1>;
    using RefLoad1 = SimpleInstruction<0x2b, 0, 0, 1>;
    using RefLoad2 = SimpleInstruction<0x2c, 0, 0, 1>;
    using RefLoad3 = SimpleInstruction<0x2d, 0, 0, 1>;
    using RefNewArray = SimpleInstruction<0xbd, 2, 1, 1>;
    using RefReturn = SimpleInstruction<0xb0, 0, 1, 0>;
    using ArrayLength = SimpleInstruction<0xbe, 0, 1, 1>;
    using RefStore = SimpleInstruction<0x3a, 1, 1, 0>;
    using RefStore0 = SimpleInstruction<0x4b, 0, 1, 0>;
    using RefStore1 = SimpleInstruction<0x4c, 0, 1, 0>;
    using RefStore2 = SimpleInstruction<0x4d, 0, 1, 0>;
    using RefStore3 = SimpleInstruction<0x4e, 0, 1, 0>;
    using RefThrow = SimpleInstruction<0xbf, 0, 1, 1>;
    using ByteArrayLoad = SimpleInstruction<0x33, 0, 2, 1>;
    using ByteArrayStore = SimpleInstruction<0x54, 0, 3, 0>;
    using BytePush = SimpleInstruction<0x10, 1, 0, 1>;
    using CharArrayLoad = SimpleInstruction<0x34, 0, 2, 1>;
    using CharArrayStore = SimpleInstruction<0x55, 0, 3, 0>;
    using CheckCast = SimpleInstruction<0xc0, 2, 1, 1>;
    using DoubleToFloat = SimpleInstruction<0x90, 0, 1, 1>;
    using DoubleToInt = SimpleInstruction<0x82, 0, 1, 1>;
    using DoubleToLong = SimpleInstruction<0x8f, 0, 1, 1>;
    using DoubleAdd = SimpleInstruction<0x63, 0, 2, 1>;
    using DoubleArrayLoad = SimpleInstruction<0x31, 0, 2, 1>;
    using DoubleArrayStore = SimpleInstruction<0x52, 0, 3, 0>;
    using DoubleCmpL = SimpleInstruction<0x97, 0, 2, 1>;
    using DoubleCmpG = SimpleInstruction<0x98, 0, 2, 1>;
    using DoubleConst0 = SimpleInstruction<0xe, 0, 0, 1>;
    using DoubleConst1 = SimpleInstruction<0xf, 0, 0, 1>;
    using DoubleDiv = SimpleInstruction<0x6f, 0, 2, 1>;
    using DoubleLoad = SimpleInstruction<0x18, 1, 0, 1>;
    using DoubleLoad0 = SimpleInstruction<0x26, 0, 0, 1>;
    using DoubleLoad1 = SimpleInstruction<0x27, 0, 0, 1>;
    using DoubleLoad2 = SimpleInstruction<0x28, 0, 0, 1>;
    using DoubleLoad3 = SimpleInstruction<0x29, 0, 0, 1>;
    using DoubleMul = SimpleInstruction<0x6b, 0, 2, 1>;
    using DoubleNeg = SimpleInstruction<0x77, 0, 1, 1>;
    using DoubleRem = SimpleInstruction<0x73, 0, 2, 1>;
    using DoubleReturn = SimpleInstruction<0xaf, 0, 1, 0>;
    using DoubleStore = SimpleInstruction<0x39, 1, 1, 0>;
    using DoubleStore0 = SimpleInstruction<0x47, 0, 1, 0>;
    using DoubleStore1 = SimpleInstruction<0x48, 0, 1, 0>;
    using DoubleStore2 = SimpleInstruction<0x49, 0, 1, 0>;
    using DoubleStore3 = SimpleInstruction<0x4a, 0, 1, 0>;
    using DoubleSub = SimpleInstruction<0x67, 0, 2, 1>;
    using Duplicate = SimpleInstruction<0x59, 0, 1, 2>;
    using DuplicateX1 = SimpleInstruction<0x5a, 0, 2, 3>;
    using DuplicateX2 = SimpleInstruction<0x5b, 0, 3, 4>;
    using Duplicate2 = SimpleInstruction<0x5c, 0, 2, 4>;
    using Duplicate2X1 = SimpleInstruction<0x5d, 0, 3, 5>;
    using Duplicate2X2 = SimpleInstruction<0x5e, 0, 4, 6>;
    using FloatToDouble = SimpleInstruction<0x8d, 0, 1, 1>;
    using FloatToInt = SimpleInstruction<0x8b, 0, 1, 1>;
    using FloatToLong = SimpleInstruction<0x8c, 0, 1, 1>;
    using FloatAdd = SimpleInstruction<0x62, 0, 2, 1>;
    using FloatArrayLoad = SimpleInstruction<0x30, 0, 2, 1>;
    using FloatArrayStore = SimpleInstruction<0x51, 0, 3, 0>;
    using FloatCmpL = SimpleInstruction<0x95, 0, 2, 1>;
    using FloatCmpG = SimpleInstruction<0x96, 0, 2, 1>;
    using FloatConst0 = SimpleInstruction<0xb, 0, 0, 1>;
    using FloatConst1 = SimpleInstruction<0xc, 0, 0, 1>;
    using FloatConst2 = SimpleInstruction<0xd, 0, 0, 1>;
    using FloatDiv = SimpleInstruction<0x6e, 0, 2, 1>;
    using FloatLoad = SimpleInstruction<0x96, 1, 0, 1>;
    using FloatLoad0 = SimpleInstruction<0x22, 0, 0, 1>;
    using FloatLoad1 = SimpleInstruction<0x23, 0, 0, 1>;
    using FloatLoad2 = SimpleInstruction<0x24, 0, 0, 1>;
    using FloatLoad3 = SimpleInstruction<0x25, 0, 0, 1>;
    using FloatMul = SimpleInstruction<0x6a, 0, 2, 1>;
    using FloatNeg = SimpleInstruction<0x76, 0, 1, 1>;
    using FloatRen = SimpleInstruction<0x72, 0, 2, 1>;
    using FloatReturn = SimpleInstruction<0xae, 0, 1, 0>;
    using FloatStore = SimpleInstruction<0x38, 1, 1, 0>;
    using FloatStore0 = SimpleInstruction<0x43, 0, 1, 0>;
    using FloatStore1 = SimpleInstruction<0x44, 0, 1, 0>;
    using FloatStore2 = SimpleInstruction<0x45, 0, 1, 0>;
    using FloatStore3 = SimpleInstruction<0x46, 0, 1, 0>;
    using FloatSub = SimpleInstruction<0x66, 0, 2, 1>;
    using GetField = SimpleInstruction<0xb4, 2, 1, 1>;
    using GetStatic = SimpleInstruction<0xb2, 2, 0, 1>;
    using GoTo = SimpleInstruction<0xa7, 2, 0, 0>;
    using GoToW = SimpleInstruction<0xc8, 4, 0, 0>;
    using IntToByte = SimpleInstruction<0x91, 0, 1, 1>;
    using IntToChar = SimpleInstruction<0x92, 0, 1, 1>;
    using IntToDouble = SimpleInstruction<0x87, 0, 1, 1>;
    using IntToFloat = SimpleInstruction<0x86, 0, 1, 1>;
    using IntToLong = SimpleInstruction<0x85, 0, 1, 1>;
    using IntToShort = SimpleInstruction<0x93, 0, 1, 1>;
    using IntAdd = SimpleInstruction<0x60, 0, 2, 1>;
    using IntArrayLoad = SimpleInstruction<0x2e, 0, 2, 1>;
    using IntAnd = SimpleInstruction<0x7e, 0, 2, 1>;
    using IntArrayStore = SimpleInstruction<0x4f, 0, 3, 0>;
    using IntConstNeg1 = SimpleInstruction<0x2, 0, 0, 1>;
    using IntConst0 = SimpleInstruction<0x3, 0, 0, 1>;
    using IntConst1 = SimpleInstruction<0x4, 0, 0, 1>;
    using IntConst2 = SimpleInstruction<0x5, 0, 0, 1>;
    using IntConst3 = SimpleInstruction<0x6, 0, 0, 1>;
    using IntConst4 = SimpleInstruction<0x7, 0, 0, 1>;
    using IntConst5 = SimpleInstruction<0x8, 0, 0, 1>;
    using IntDiv = SimpleInstruction<0x6c, 0, 2, 1>;
    using IfRefCmpEq = SimpleInstruction<0xa5, 2, 2, 0>;
    using IfRefCmpNe = SimpleInstruction<0xa6, 2, 2, 0>;
    using IfIntCmpEq = SimpleInstruction<0x9f, 2, 2, 0>;
    using IfIntCmpNe = SimpleInstruction<0xa0, 2, 2, 0>;
    using IfIntCmpLt = SimpleInstruction<0xa1, 2, 2, 0>;
    using IfIntCmpGe = SimpleInstruction<0xa2, 2, 2, 0>;
    using IfIntCmpGt = SimpleInstruction<0xa3, 2, 2, 0>;
    using IfIntCmpLe = SimpleInstruction<0xa4, 2, 2, 0>;
    using IfEq = SimpleInstruction<0x99, 2, 1, 0>;
    using IfNe = SimpleInstruction<0x9a, 2, 1, 0>;
    using IfLt = SimpleInstruction<0x9b, 2, 1, 0>;
    using IfGe = SimpleInstruction<0x9c, 2, 1, 0>;
    using IfGt = SimpleInstruction<0x9d, 2, 1, 0>;
    using IfLe = SimpleInstruction<0x9e, 2, 1, 0>;
    using IfNonNull = SimpleInstruction<0xc7, 2, 1, 0>;
    using IfNull = SimpleInstruction<0xc6, 2, 1, 0>;
    using IntInc = SimpleInstruction<0x84, 2, 0, 0>;
    using IntLoad = SimpleInstruction<0x15, 1, 0, 1>;
    using IntLoad0 = SimpleInstruction<0x1a, 0, 0, 1>;
    using IntLoad1 = SimpleInstruction<0x1b, 0, 0, 1>;
    using IntLoad2 = SimpleInstruction<0x1c, 0, 0, 1>;
    using IntLoad3 = SimpleInstruction<0x1d, 0, 0, 1>;
    using IntMul = SimpleInstruction<0x68, 0, 2, 1>;
    using IntNeg = SimpleInstruction<0x74, 0, 1, 1>;
    using InstanceOf = SimpleInstruction<0xc1, 2, 1, 1>;
    using InvokeDynamic = SimpleInstruction<0xba, 4, 0, 0>;
    using InvokeInterface = SimpleInstruction<0xb9, 4, 1, 0>;
    using InvokeSpecial = SimpleInstruction<0xb7, 2, 1, 0>;
    using InvokeStatic = SimpleInstruction<0xb8, 2, 0, 0>;
    using InvokeVirtual = SimpleInstruction<0xb6, 2, 1, 0>;
    using IntOr = SimpleInstruction<0x80, 0, 2, 1>;
    using IntRem = SimpleInstruction<0x70, 0, 2, 1>;
    using IntReturn = SimpleInstruction<0xac, 0, 1, 0>;
    using IntShl = SimpleInstruction<0x78, 0, 2, 1>;
    using IntShr = SimpleInstruction<0x7a, 0, 2, 1>;
    using IntStore = SimpleInstruction<0x36, 1, 1, 0>;
    using IntStore0 = SimpleInstruction<0x3b, 0, 1, 0>;
    using IntStore1 = SimpleInstruction<0x3c, 0, 1, 0>;
    using IntStore2 = SimpleInstruction<0x3d, 0, 1, 0>;
    using IntStore3 = SimpleInstruction<0x3e, 0, 1, 0>;
    using IntSub = SimpleInstruction<0x64, 0, 2, 1>;
    using IntUShr = SimpleInstruction<0x7c, 0, 2, 1>;
    using IntXor = SimpleInstruction<0x82, 0, 2, 1>;
    using JmpSubroutine = SimpleInstruction<0xa8, 2, 0, 1>;
    using JmpSubroutineW = SimpleInstruction<0xc9, 4, 0, 1>;
    using LongToDouble = SimpleInstruction<0x8a, 0, 1, 1>;
    using LongToFloat = SimpleInstruction<0x89, 0, 1, 1>;
    using LongToInt = SimpleInstruction<0x88, 0, 1, 1>;
    using LongAdd = SimpleInstruction<0x61, 0, 2, 1>;
    using LongArrayLoad = SimpleInstruction<0x2f, 0, 2, 1>;
    using LongAnd = SimpleInstruction<0x7f, 0, 2, 1>;
    using LongArrayStore = SimpleInstruction<0x50, 0, 3, 0>;
    using LongCmp = SimpleInstruction<0x94, 0, 2, 1>;
    using LongConst0 = SimpleInstruction<0x9, 0, 0, 1>;
    using LongConst1 = SimpleInstruction<0xa, 0, 0, 1>;
    using LoadConst = SimpleInstruction<0x12, 1, 0, 1>;
    using LoadConstW = SimpleInstruction<0x13, 2, 0, 1>;
    using LoadConst2W = SimpleInstruction<0x14, 2, 0, 1>;
    using LongDiv = SimpleInstruction<0x6d, 0, 2, 1>;
    using LongLoad = SimpleInstruction<0x16, 1, 0, 1>;
    using LongLoad0 = SimpleInstruction<0x1e, 0, 0, 1>;
    using LongLoad1 = SimpleInstruction<0x1f, 0, 0, 1>;
    using LongLoad2 = SimpleInstruction<0x20, 0, 0, 1>;
    using LongLoad3 = SimpleInstruction<0x21, 0, 0, 1>;
    using LongMul = SimpleInstruction<0x69, 0, 2, 1>;
    using LongNeg = SimpleInstruction<0x75, 0, 1, 1>;
    using LongOr = SimpleInstruction<0x81, 0, 2, 1>;
    using LongRem = SimpleInstruction<0x71, 0, 2, 1>;
    using LongReturn = SimpleInstruction<0xad, 0, 1, 0>;
    using LongShl = SimpleInstruction<0x79, 0, 2, 1>;
    using LongShr = SimpleInstruction<0x7b, 0, 2, 1>;
    using LongStore = SimpleInstruction<0x37, 1, 1, 0>;
    using LongStore0 = SimpleInstruction<0x3f, 0, 1, 0>;
    using LongStore1 = SimpleInstruction<0x40, 0, 1, 0>;
    using LongStore2 = SimpleInstruction<0x41, 0, 1, 0>;
    using LongStore3 = SimpleInstruction<0x42, 0, 1, 0>;
    using LongSub = SimpleInstruction<0x65, 0, 2, 1>;
    using LongUShr = SimpleInstruction<0x7d, 0, 2, 1>;
    using LongXor = SimpleInstruction<0x83, 0, 2, 1>;
    using MonitorEnter = SimpleInstruction<0xc2, 0, 1, 0>;
    using MonitorExit = SimpleInstruction<0xc3, 0, 1, 0>;
    using MultiRefNewArray = SimpleInstruction<0xc5, 3, 1, 1>;
    using New = SimpleInstruction<0xbb, 2, 0, 1>;
    using NewArray = SimpleInstruction<0xbc, 1, 1, 1>;
    using Nop = SimpleInstruction<0x0, 0, 0, 0>;
    using Pop = SimpleInstruction<0x57, 0, 1, 0>;
    using Pop2 = SimpleInstruction<0x58, 0, 2, 0>;
    using PutField = SimpleInstruction<0xb5, 2, 2, 0>;
    using PutStatic = SimpleInstruction<0xb3, 2, 1, 0>;
    using Ret = SimpleInstruction<0xa9, 1, 0, 0>;
    using Return = SimpleInstruction<0xb1, 0, 0, 0>;
    using ShortArrayLoad = SimpleInstruction<0x35, 0, 2, 1>;
    using ShortArrayStore = SimpleInstruction<0x56, 0, 3, 0>;
    using ShortPush = SimpleInstruction<0x11, 2, 0, 1>;
    using Swap = SimpleInstruction<0x5f, 0, 2, 2>;

    // TODO: following instructions cannot be implemented using the same simple
    //       instruction format as the instructions above:
    //         - lookup switch instruction
    //         - table switch instruction
    //         - wide instruction

}

#endif //JAWA_INSTRUCTION_HPP
