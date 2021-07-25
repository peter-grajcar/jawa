/**
 * @file instruction.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_INSTRUCTION_HPP
#define JAWA_INSTRUCTION_HPP

#include <array>
#include <initializer_list>
#include <iostream>

#include "byte_code.hpp"
#include "constant_pool.hpp"
#include "mnemonics.hpp"

#define JASM_SPECIALISATION(X)                                                                                         \
    template<>                                                                                                         \
    void X::jasm(std::ostream &os, const ConstantPool *pool) const;

namespace jasm {

    using namespace byte_code;

    /**
     * operand count, stack input, stack output
     */
    constexpr u2 InstructionInfo[][3] = {
        { 0, 0, 0 }, // 0x00 nop
        { 0, 0, 1 }, // 0x01 aconst_null
        { 0, 0, 1 }, // 0x02 iconst_m1
        { 0, 0, 1 }, // 0x03 iconst_0
        { 0, 0, 1 }, // 0x04 iconst_1
        { 0, 0, 1 }, // 0x05 iconst_2
        { 0, 0, 1 }, // 0x06 iconst_3
        { 0, 0, 1 }, // 0x07 iconst_4
        { 0, 0, 1 }, // 0x08 iconst_5
        { 0, 0, 1 }, // 0x09 lconst_0
        { 0, 0, 1 }, // 0x0a lconst_1
        { 0, 0, 1 }, // 0x0b fconst_0
        { 0, 0, 1 }, // 0x0c fconst_1
        { 0, 0, 1 }, // 0x0d fconst_2
        { 0, 0, 1 }, // 0x0e dconst_0
        { 0, 0, 1 }, // 0x0f dconst_1
        { 1, 0, 1 }, // 0x10 bipush
        { 2, 0, 1 }, // 0x11 sipush
        { 1, 0, 1 }, // 0x12 ldc
        { 2, 0, 1 }, // 0x13 ldc_w
        { 2, 0, 1 }, // 0x14 ldc2_w
        { 1, 0, 1 }, // 0x15 iload
        { 1, 0, 1 }, // 0x16 lload
        { 1, 0, 1 }, // 0x17 fload
        { 1, 0, 1 }, // 0x18 dload
        { 1, 0, 1 }, // 0x19 aload
        { 0, 0, 1 }, // 0x1a iload_0
        { 0, 0, 1 }, // 0x1b iload_1
        { 0, 0, 1 }, // 0x1c iload_2
        { 0, 0, 1 }, // 0x1d iload_3
        { 0, 0, 1 }, // 0x1e lload_0
        { 0, 0, 1 }, // 0x1f lload_1
        { 0, 0, 1 }, // 0x20 lload_2
        { 0, 0, 1 }, // 0x21 lload_3
        { 0, 0, 1 }, // 0x22 fload_0
        { 0, 0, 1 }, // 0x23 fload_1
        { 0, 0, 1 }, // 0x24 fload_2
        { 0, 0, 1 }, // 0x25 fload_3
        { 0, 0, 1 }, // 0x26 dload_0
        { 0, 0, 1 }, // 0x27 dload_1
        { 0, 0, 1 }, // 0x28 dload_2
        { 0, 0, 1 }, // 0x29 dload_3
        { 0, 0, 1 }, // 0x2a aload_0
        { 0, 0, 1 }, // 0x2b aload_1
        { 0, 0, 1 }, // 0x2c aload_2
        { 0, 0, 1 }, // 0x2d aload_3
        { 0, 2, 1 }, // 0x2e iaload
        { 0, 2, 1 }, // 0x2f laload
        { 0, 2, 1 }, // 0x30 faload
        { 0, 2, 1 }, // 0x31 daload
        { 0, 2, 1 }, // 0x32 aaload
        { 0, 2, 1 }, // 0x33 baload
        { 0, 2, 1 }, // 0x34 caload
        { 0, 2, 1 }, // 0x35 saload
        { 1, 1, 0 }, // 0x36 istore
        { 1, 1, 0 }, // 0x37 lstore
        { 1, 1, 0 }, // 0x38 fstore
        { 1, 1, 0 }, // 0x39 dstore
        { 1, 1, 0 }, // 0x3a astore
        { 0, 1, 0 }, // 0x3b istore_0
        { 0, 1, 0 }, // 0x3c istore_1
        { 0, 1, 0 }, // 0x3d istore_2
        { 0, 1, 0 }, // 0x3e istore_3
        { 0, 1, 0 }, // 0x3f lstore_0
        { 0, 1, 0 }, // 0x40 lstore_1
        { 0, 1, 0 }, // 0x41 lstore_2
        { 0, 1, 0 }, // 0x42 lstore_3
        { 0, 1, 0 }, // 0x43 fstore_0
        { 0, 1, 0 }, // 0x44 fstore_1
        { 0, 1, 0 }, // 0x45 fstore_2
        { 0, 1, 0 }, // 0x46 fstore_3
        { 0, 1, 0 }, // 0x47 dstore_0
        { 0, 1, 0 }, // 0x48 dstore_1
        { 0, 1, 0 }, // 0x49 dstore_2
        { 0, 1, 0 }, // 0x4a dstore_3
        { 0, 1, 0 }, // 0x4b astore_0
        { 0, 1, 0 }, // 0x4c astore_1
        { 0, 1, 0 }, // 0x4d astore_2
        { 0, 1, 0 }, // 0x4e astore_3
        { 0, 3, 0 }, // 0x4f iastore
        { 0, 3, 0 }, // 0x50 lastore
        { 0, 3, 0 }, // 0x51 fastore
        { 0, 3, 0 }, // 0x52 dastore
        { 0, 3, 0 }, // 0x53 aastore
        { 0, 3, 0 }, // 0x54 bastore
        { 0, 3, 0 }, // 0x55 castore
        { 0, 3, 0 }, // 0x56 sastore
        { 0, 1, 0 }, // 0x57 pop
        { 0, 2, 0 }, // 0x58 pop2
        { 0, 1, 2 }, // 0x59 dup
        { 0, 2, 3 }, // 0x5a dup_x1
        { 0, 3, 4 }, // 0x5b dup_x2
        { 0, 2, 4 }, // 0x5c dup2
        { 0, 3, 5 }, // 0x5d dup2_x1
        { 0, 4, 6 }, // 0x5e dup2_x2
        { 0, 2, 2 }, // 0x5f swap
        { 0, 2, 1 }, // 0x60 iadd
        { 0, 2, 1 }, // 0x61 0x61
        { 0, 2, 1 }, // 0x62 0x62
        { 0, 2, 1 }, // 0x63 0x63
        { 0, 2, 1 }, // 0x64 isub
        { 0, 2, 1 }, // 0x65 lsub
        { 0, 2, 1 }, // 0x66 fsub
        { 0, 2, 1 }, // 0x67 dsub
        { 0, 2, 1 }, // 0x68 imul
        { 0, 2, 1 }, // 0x69 lmul
        { 0, 2, 1 }, // 0x6a fmul
        { 0, 2, 1 }, // 0x6b dmul
        { 0, 2, 1 }, // 0x6c idiv
        { 0, 2, 1 }, // 0x6d ldiv
        { 0, 2, 1 }, // 0x6e fdiv
        { 0, 2, 1 }, // 0x6f ddiv
        { 0, 2, 1 }, // 0x70 irem
        { 0, 2, 1 }, // 0x71 lrem
        { 0, 2, 1 }, // 0x72 frem
        { 0, 2, 1 }, // 0x73 drem
        { 0, 1, 1 }, // 0x74 ineg
        { 0, 1, 1 }, // 0x75 lneg
        { 0, 1, 1 }, // 0x76 fneg
        { 0, 1, 1 }, // 0x77 dneg
        { 0, 2, 1 }, // 0x78 ishl
        { 0, 2, 1 }, // 0x79 lshl
        { 0, 2, 1 }, // 0x7a ishr
        { 0, 2, 1 }, // 0x7b lshr
        { 0, 2, 1 }, // 0x7c iushr
        { 0, 2, 1 }, // 0x7d lushr
        { 0, 2, 1 }, // 0x7e iand
        { 0, 2, 1 }, // 0x7f land
        { 0, 2, 1 }, // 0x80 ior
        { 0, 2, 1 }, // 0x81 lor
        { 0, 2, 1 }, // 0x82 ixor
        { 0, 2, 1 }, // 0x83 lxor
        { 2, 0, 0 }, // 0x84 iinc
        { 0, 1, 1 }, // 0x85 i2l
        { 0, 1, 1 }, // 0x86 i2f
        { 0, 1, 1 }, // 0x87 i2d
        { 0, 1, 1 }, // 0x88 l2i
        { 0, 1, 1 }, // 0x89 l2f
        { 0, 1, 1 }, // 0x8a l2d
        { 0, 1, 1 }, // 0x8b f2i
        { 0, 1, 1 }, // 0x8c f2l
        { 0, 1, 1 }, // 0x8d f2d
        { 0, 1, 1 }, // 0x8e d2i
        { 0, 1, 1 }, // 0x8f d2l
        { 0, 1, 1 }, // 0x90 d2f
        { 0, 1, 1 }, // 0x91 i2b
        { 0, 1, 1 }, // 0x92 i2c
        { 0, 1, 1 }, // 0x93 i2s
        { 0, 2, 1 }, // 0x94 lcmp
        { 0, 2, 1 }, // 0x95 fcmpl
        { 0, 2, 1 }, // 0x96 fcmpg
        { 0, 2, 1 }, // 0x97 dcmpl
        { 0, 2, 1 }, // 0x98 dcmpg
        { 2, 1, 0 }, // 0x99 ifeq
        { 2, 1, 0 }, // 0x9a ifne
        { 2, 1, 0 }, // 0x9b iflt
        { 2, 1, 0 }, // 0x9c ifge
        { 2, 1, 0 }, // 0x9d ifgt
        { 2, 1, 0 }, // 0x9e ifle
        { 2, 2, 0 }, // 0x9f if_icmpeq
        { 2, 2, 0 }, // 0xa0 if_icmpne
        { 2, 2, 0 }, // 0xa1 if_icmplt
        { 2, 2, 0 }, // 0xa2 if_icmpge
        { 2, 2, 0 }, // 0xa3 if_icmpgt
        { 2, 2, 0 }, // 0xa4 if_icmple
        { 2, 2, 0 }, // 0xa5 if_acmpeq
        { 2, 2, 0 }, // 0xa6 if_acmpne
        { 2, 0, 0 }, // 0xa7 goto
        { 2, 0, 1 }, // 0xa8 jsr
        { 1, 0, 0 }, // 0xa9 ret
        { 0, 0, 0 }, // 0xaa tableswitch
        { 0, 0, 0 }, // axab lookupswitch
        { 0, 1, 0 }, // 0xac ireturn
        { 0, 1, 0 }, // 0xad lreturn
        { 0, 1, 0 }, // 0xae freturn
        { 0, 1, 0 }, // 0xaf dreturn
        { 0, 1, 0 }, // 0xb0 areturn
        { 0, 0, 0 }, // 0xb1 return
        { 2, 0, 1 }, // 0xb2 getstatic
        { 2, 1, 0 }, // 0xb3 putstatic
        { 2, 1, 1 }, // 0xb4 getfield
        { 2, 2, 0 }, // 0xb5 putfield
        { 2, 1, 0 }, // 0xb6 invokevirtual
        { 2, 1, 0 }, // 0xb7 invokespecial
        { 2, 0, 0 }, // 0xb8 invokestatic
        { 4, 1, 0 }, // 0xb9 invokeinterface
        { 4, 0, 0 }, // 0xba invokedynamic
        { 2, 0, 1 }, // 0xbb new
        { 1, 1, 1 }, // 0xbc newarray
        { 2, 1, 1 }, // 0xbd anewarray
        { 0, 1, 1 }, // 0xbe arraylength
        { 0, 1, 1 }, // 0xbf athrow
        { 2, 1, 1 }, // 0xc0 checkcast
        { 2, 1, 1 }, // 0xc1 instanceof
        { 0, 1, 0 }, // 0xc2 monitorenter
        { 0, 1, 0 }, // 0xc3 monitorexit
        { 0, 0, 0 }, // 0xc4 wide
        { 3, 1, 1 }, // 0xc5 multianewarray
        { 2, 1, 0 }, // 0xc6 ifnull
        { 2, 1, 0 }, // 0xc7 ifnonnull
        { 4, 0, 0 }, // 0xc8 goto_w
        { 4, 0, 1 }, // 0xc9 jsr_w
        { 0, 0, 0 }, // 0xca breakpoint
    };

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

        virtual u4 size() const = 0;

        virtual void jasm(std::ostream &os, const ConstantPool *pool) const = 0;

        virtual void emit_bytecode(std::ostream &os) const = 0;
    };

    template<u1 opcode_>
    class SimpleInstruction : public Instruction
    {
        std::array<u1, InstructionInfo[opcode_][0]> operands_;

    public:
        template<typename... Args>
        explicit SimpleInstruction(Args... args)
          : operands_{ args... }
        {
            assert(opcode_ != 0xc4 && opcode_ != 0xaa && opcode_ != 0xab);
        }

        SimpleInstruction(std::initializer_list<u1> args)
          : operands_(args)
        {
            assert(opcode_ != 0xc4 && opcode_ != 0xaa && opcode_ != 0xab);
        }

        explicit SimpleInstruction(std::istream *is)
        {
            assert(opcode_ != 0xc4 && opcode_ != 0xaa && opcode_ != 0xab);
            assert(is);
            for (u2 i = 0; i < InstructionInfo[opcode_][0]; ++i)
                operands_[i] = read_big_endian<u1>(*is);
        }

        inline u1 opcode() const override { return opcode_; }

        inline const char *mnemonic() const override { return InstructionMnemonics[opcode_]; }

        inline u2 operand_count() const override { return InstructionInfo[opcode_][0]; }

        inline u2 input_stack_operand_count() const override { return InstructionInfo[opcode_][1]; }

        inline u2 output_stack_operand_count() const override { return InstructionInfo[opcode_][2]; }

        inline u4 size() const override { return 1 + operand_count(); }

        void jasm(std::ostream &os, const ConstantPool *pool) const override
        {
            os << std::setw(19) << mnemonic();
            for (u1 op : operands_)
                os << " $" << (int) op;
            os << std::endl;
        }

        void emit_bytecode(std::ostream &os) const override
        {
            write_big_endian<u1>(os, opcode_);
            for (auto operand : operands_)
                write_big_endian<u1>(os, operand);
        }
    };

    using RefArrayLoad = SimpleInstruction<0x32>;
    using RefArrayStore = SimpleInstruction<0x53>;
    using RefConstNull = SimpleInstruction<0x1>;
    using RefLoad = SimpleInstruction<0x19>;
    using RefLoad0 = SimpleInstruction<0x2a>;
    using RefLoad1 = SimpleInstruction<0x2b>;
    using RefLoad2 = SimpleInstruction<0x2c>;
    using RefLoad3 = SimpleInstruction<0x2d>;
    using RefNewArray = SimpleInstruction<0xbd>;
    using RefReturn = SimpleInstruction<0xb0>;
    using ArrayLength = SimpleInstruction<0xbe>;
    using RefStore = SimpleInstruction<0x3a>;
    using RefStore0 = SimpleInstruction<0x4b>;
    using RefStore1 = SimpleInstruction<0x4c>;
    using RefStore2 = SimpleInstruction<0x4d>;
    using RefStore3 = SimpleInstruction<0x4e>;
    using RefThrow = SimpleInstruction<0xbf>;
    using ByteArrayLoad = SimpleInstruction<0x33>;
    using ByteArrayStore = SimpleInstruction<0x54>;
    using BytePush = SimpleInstruction<0x10>;
    using CharArrayLoad = SimpleInstruction<0x34>;
    using CharArrayStore = SimpleInstruction<0x55>;
    using CheckCast = SimpleInstruction<0xc0>;
    using DoubleToFloat = SimpleInstruction<0x90>;
    using DoubleToInt = SimpleInstruction<0x8e>;
    using DoubleToLong = SimpleInstruction<0x8f>;
    using DoubleAdd = SimpleInstruction<0x63>;
    using DoubleArrayLoad = SimpleInstruction<0x31>;
    using DoubleArrayStore = SimpleInstruction<0x52>;
    using DoubleCmpL = SimpleInstruction<0x97>;
    using DoubleCmpG = SimpleInstruction<0x98>;
    using DoubleConst0 = SimpleInstruction<0xe>;
    using DoubleConst1 = SimpleInstruction<0xf>;
    using DoubleDiv = SimpleInstruction<0x6f>;
    using DoubleLoad = SimpleInstruction<0x18>;
    using DoubleLoad0 = SimpleInstruction<0x26>;
    using DoubleLoad1 = SimpleInstruction<0x27>;
    using DoubleLoad2 = SimpleInstruction<0x28>;
    using DoubleLoad3 = SimpleInstruction<0x29>;
    using DoubleMul = SimpleInstruction<0x6b>;
    using DoubleNeg = SimpleInstruction<0x77>;
    using DoubleRem = SimpleInstruction<0x73>;
    using DoubleReturn = SimpleInstruction<0xaf>;
    using DoubleStore = SimpleInstruction<0x39>;
    using DoubleStore0 = SimpleInstruction<0x47>;
    using DoubleStore1 = SimpleInstruction<0x48>;
    using DoubleStore2 = SimpleInstruction<0x49>;
    using DoubleStore3 = SimpleInstruction<0x4a>;
    using DoubleSub = SimpleInstruction<0x67>;
    using Duplicate = SimpleInstruction<0x59>;
    using DuplicateX1 = SimpleInstruction<0x5a>;
    using DuplicateX2 = SimpleInstruction<0x5b>;
    using Duplicate2 = SimpleInstruction<0x5c>;
    using Duplicate2X1 = SimpleInstruction<0x5d>;
    using Duplicate2X2 = SimpleInstruction<0x5e>;
    using FloatToDouble = SimpleInstruction<0x8d>;
    using FloatToInt = SimpleInstruction<0x8b>;
    using FloatToLong = SimpleInstruction<0x8c>;
    using FloatAdd = SimpleInstruction<0x62>;
    using FloatArrayLoad = SimpleInstruction<0x30>;
    using FloatArrayStore = SimpleInstruction<0x51>;
    using FloatCmpL = SimpleInstruction<0x95>;
    using FloatCmpG = SimpleInstruction<0x96>;
    using FloatConst0 = SimpleInstruction<0xb>;
    using FloatConst1 = SimpleInstruction<0xc>;
    using FloatConst2 = SimpleInstruction<0xd>;
    using FloatDiv = SimpleInstruction<0x6e>;
    using FloatLoad = SimpleInstruction<0x17>;
    using FloatLoad0 = SimpleInstruction<0x22>;
    using FloatLoad1 = SimpleInstruction<0x23>;
    using FloatLoad2 = SimpleInstruction<0x24>;
    using FloatLoad3 = SimpleInstruction<0x25>;
    using FloatMul = SimpleInstruction<0x6a>;
    using FloatNeg = SimpleInstruction<0x76>;
    using FloatRen = SimpleInstruction<0x72>;
    using FloatReturn = SimpleInstruction<0xae>;
    using FloatStore = SimpleInstruction<0x38>;
    using FloatStore0 = SimpleInstruction<0x43>;
    using FloatStore1 = SimpleInstruction<0x44>;
    using FloatStore2 = SimpleInstruction<0x45>;
    using FloatStore3 = SimpleInstruction<0x46>;
    using FloatSub = SimpleInstruction<0x66>;
    using GetField = SimpleInstruction<0xb4>;
    using GetStatic = SimpleInstruction<0xb2>;
    using GoTo = SimpleInstruction<0xa7>;
    using GoToW = SimpleInstruction<0xc8>;
    using IntToByte = SimpleInstruction<0x91>;
    using IntToChar = SimpleInstruction<0x92>;
    using IntToDouble = SimpleInstruction<0x87>;
    using IntToFloat = SimpleInstruction<0x86>;
    using IntToLong = SimpleInstruction<0x85>;
    using IntToShort = SimpleInstruction<0x93>;
    using IntAdd = SimpleInstruction<0x60>;
    using IntArrayLoad = SimpleInstruction<0x2e>;
    using IntAnd = SimpleInstruction<0x7e>;
    using IntArrayStore = SimpleInstruction<0x4f>;
    using IntConstNeg1 = SimpleInstruction<0x2>;
    using IntConst0 = SimpleInstruction<0x3>;
    using IntConst1 = SimpleInstruction<0x4>;
    using IntConst2 = SimpleInstruction<0x5>;
    using IntConst3 = SimpleInstruction<0x6>;
    using IntConst4 = SimpleInstruction<0x7>;
    using IntConst5 = SimpleInstruction<0x8>;
    using IntDiv = SimpleInstruction<0x6c>;
    using IfRefCmpEq = SimpleInstruction<0xa5>;
    using IfRefCmpNe = SimpleInstruction<0xa6>;
    using IfIntCmpEq = SimpleInstruction<0x9f>;
    using IfIntCmpNe = SimpleInstruction<0xa0>;
    using IfIntCmpLt = SimpleInstruction<0xa1>;
    using IfIntCmpGe = SimpleInstruction<0xa2>;
    using IfIntCmpGt = SimpleInstruction<0xa3>;
    using IfIntCmpLe = SimpleInstruction<0xa4>;
    using IfEq = SimpleInstruction<0x99>;
    using IfNe = SimpleInstruction<0x9a>;
    using IfLt = SimpleInstruction<0x9b>;
    using IfGe = SimpleInstruction<0x9c>;
    using IfGt = SimpleInstruction<0x9d>;
    using IfLe = SimpleInstruction<0x9e>;
    using IfNonNull = SimpleInstruction<0xc7>;
    using IfNull = SimpleInstruction<0xc6>;
    using IntInc = SimpleInstruction<0x84>;
    using IntLoad = SimpleInstruction<0x15>;
    using IntLoad0 = SimpleInstruction<0x1a>;
    using IntLoad1 = SimpleInstruction<0x1b>;
    using IntLoad2 = SimpleInstruction<0x1c>;
    using IntLoad3 = SimpleInstruction<0x1d>;
    using IntMul = SimpleInstruction<0x68>;
    using IntNeg = SimpleInstruction<0x74>;
    using InstanceOf = SimpleInstruction<0xc1>;
    using InvokeDynamic = SimpleInstruction<0xba>;
    using InvokeInterface = SimpleInstruction<0xb9>;
    using InvokeSpecial = SimpleInstruction<0xb7>;
    using InvokeStatic = SimpleInstruction<0xb8>;
    using InvokeVirtual = SimpleInstruction<0xb6>;
    using IntOr = SimpleInstruction<0x80>;
    using IntRem = SimpleInstruction<0x70>;
    using IntReturn = SimpleInstruction<0xac>;
    using IntShl = SimpleInstruction<0x78>;
    using IntShr = SimpleInstruction<0x7a>;
    using IntStore = SimpleInstruction<0x36>;
    using IntStore0 = SimpleInstruction<0x3b>;
    using IntStore1 = SimpleInstruction<0x3c>;
    using IntStore2 = SimpleInstruction<0x3d>;
    using IntStore3 = SimpleInstruction<0x3e>;
    using IntSub = SimpleInstruction<0x64>;
    using IntUShr = SimpleInstruction<0x7c>;
    using IntXor = SimpleInstruction<0x82>;
    using JmpSubroutine = SimpleInstruction<0xa8>;
    using JmpSubroutineW = SimpleInstruction<0xc9>;
    using LongToDouble = SimpleInstruction<0x8a>;
    using LongToFloat = SimpleInstruction<0x89>;
    using LongToInt = SimpleInstruction<0x88>;
    using LongAdd = SimpleInstruction<0x61>;
    using LongArrayLoad = SimpleInstruction<0x2f>;
    using LongAnd = SimpleInstruction<0x7f>;
    using LongArrayStore = SimpleInstruction<0x50>;
    using LongCmp = SimpleInstruction<0x94>;
    using LongConst0 = SimpleInstruction<0x9>;
    using LongConst1 = SimpleInstruction<0xa>;
    using LoadConst = SimpleInstruction<0x12>;
    using LoadConstW = SimpleInstruction<0x13>;
    using LoadConst2W = SimpleInstruction<0x14>;
    using LongDiv = SimpleInstruction<0x6d>;
    using LongLoad = SimpleInstruction<0x16>;
    using LongLoad0 = SimpleInstruction<0x1e>;
    using LongLoad1 = SimpleInstruction<0x1f>;
    using LongLoad2 = SimpleInstruction<0x20>;
    using LongLoad3 = SimpleInstruction<0x21>;
    using LongMul = SimpleInstruction<0x69>;
    using LongNeg = SimpleInstruction<0x75>;
    using LongOr = SimpleInstruction<0x81>;
    using LongRem = SimpleInstruction<0x71>;
    using LongReturn = SimpleInstruction<0xad>;
    using LongShl = SimpleInstruction<0x79>;
    using LongShr = SimpleInstruction<0x7b>;
    using LongStore = SimpleInstruction<0x37>;
    using LongStore0 = SimpleInstruction<0x3f>;
    using LongStore1 = SimpleInstruction<0x40>;
    using LongStore2 = SimpleInstruction<0x41>;
    using LongStore3 = SimpleInstruction<0x42>;
    using LongSub = SimpleInstruction<0x65>;
    using LongUShr = SimpleInstruction<0x7d>;
    using LongXor = SimpleInstruction<0x83>;
    using MonitorEnter = SimpleInstruction<0xc2>;
    using MonitorExit = SimpleInstruction<0xc3>;
    using MultiRefNewArray = SimpleInstruction<0xc5>;
    using New = SimpleInstruction<0xbb>;
    using NewArray = SimpleInstruction<0xbc>;
    using Nop = SimpleInstruction<0x0>;
    using Pop = SimpleInstruction<0x57>;
    using Pop2 = SimpleInstruction<0x58>;
    using PutField = SimpleInstruction<0xb5>;
    using PutStatic = SimpleInstruction<0xb3>;
    using Ret = SimpleInstruction<0xa9>;
    using Return = SimpleInstruction<0xb1>;
    using ShortArrayLoad = SimpleInstruction<0x35>;
    using ShortArrayStore = SimpleInstruction<0x56>;
    using ShortPush = SimpleInstruction<0x11>;
    using Swap = SimpleInstruction<0x5f>;

    // TODO: following instructions cannot be implemented using the same simple
    //       instruction format as the instructions above:
    //         - lookup switch instruction
    //         - table switch instruction
    //         - wide instruction

    JASM_SPECIALISATION(InvokeVirtual)

    JASM_SPECIALISATION(InvokeSpecial)

    JASM_SPECIALISATION(GetStatic)

}

#endif // JAWA_INSTRUCTION_HPP
