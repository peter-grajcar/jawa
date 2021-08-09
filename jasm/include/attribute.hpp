/**
 * @file attributes.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_ATTRIBUTE_HPP
#define JAWA_ATTRIBUTE_HPP

#include <vector>

#include "byte_code.hpp"
#include "constant_pool.hpp"
#include "instruction.hpp"

namespace jasm {

    using namespace byte_code;

    class Attribute
    {
    protected:
        u2 attribute_name_index_;

    public:
        virtual ~Attribute() = default;

        explicit Attribute(u2 attribute_name_index)
          : attribute_name_index_(attribute_name_index)
        {}

        virtual void
        jasm(std::ostream &os, const ConstantPool *pool = nullptr) const = 0;

        virtual void
        emit_bytecode(std::ostream &os) const = 0;

        virtual u4
        length() const = 0;
    };

    class Attributable
    {
    protected:
        std::vector<std::unique_ptr<Attribute>> attributes_;

    public:
        Attributable() = default;

        Attributable(const Attributable &) = delete;

        Attributable(Attributable &&) = default;

        Attributable &
        operator=(const Attributable &) = delete;

        Attributable &
        operator=(Attributable &&) = default;

        virtual ~Attributable() = default;

        inline std::vector<std::unique_ptr<Attribute>> &
        attributes()
        {
            return attributes_;
        }

        inline const std::vector<std::unique_ptr<Attribute>> &
        attributes() const
        {
            return attributes_;
        }

        template<typename T, typename... Args>
        inline void
        make_attribute(Args... args)
        {
            attributes_.push_back(std::make_unique<T>(args...));
        }

        template<typename T>
        inline void
        add_attribute(T &&attr)
        {
            attributes_.push_back(std::make_unique<T>(std::forward<T>(attr)));
        }
    };

    class ConstantValueAttribute : public Attribute
    {
    private:
        u2 constant_value_index_;

    public:
        ConstantValueAttribute(u2 attribute_name_index, u2 constant_value_index)
          : Attribute(attribute_name_index)
          , constant_value_index_(constant_value_index)
        {}

        inline u4
        length() const override
        {
            return 2;
        }
    };

    class CodeAttribute
      : public Attribute
      , public Attributable
    {
    public:
        struct ExceptionTableEntry
        {
            u2 start_pc;
            u2 end_pc;
            u2 handler_pc;
            u2 catch_type;

            ExceptionTableEntry(u2 start_pc, u2 end_pc, u2 handler_pc, u2 catch_type)
              : start_pc(start_pc)
              , end_pc(end_pc)
              , handler_pc(handler_pc)
              , catch_type(catch_type)
            {}
        };

    private:
        u2 max_stack_;
        u2 max_locals_;
        std::vector<std::unique_ptr<Instruction>> code_;
        std::vector<ExceptionTableEntry> exception_table_;

        u4
        code_length() const;

        u4
        exception_table_length() const;

        u4
        attributes_length() const;

    public:
        CodeAttribute(u2 name_index, u2 max_stack, u2 max_locals)
          : Attribute(name_index)
          , max_stack_(max_stack)
          , max_locals_(max_locals)
        {}

        inline void
        make_exception_table_entry(u2 start_pc, u2 end_pc, u2 handler_pc, u2 catch_type)
        {
            exception_table_.emplace_back(start_pc, end_pc, handler_pc, catch_type);
        }

        template<typename T, typename... Args>
        inline void
        make_instruction(Args... args)
        {
            code_.emplace_back(std::make_unique<T>(args...));
        }

        template<typename T>
        inline void
        add_instruction(T &&inst)
        {
            code_.emplace_back(std::forward<T>(inst));
        }

        inline std::vector<std::unique_ptr<Instruction>> &
        code()
        {
            return code_;
        }

        inline u2
        locals_limit() const
        {
            return max_locals_;
        }

        inline u2
        stack_limit() const
        {
            return max_stack_;
        }

        inline void
        set_locals_limit(u2 max_locals)
        {
            max_locals_ = max_locals;
        }

        inline void
        set_stack_limit(u2 max_stack)
        {
            max_stack_ = max_stack;
        }

        void
        jasm(std::ostream &os, const ConstantPool *pool) const override;

        void
        emit_bytecode(std::ostream &os) const override;

        u4
        length() const override;
    };

    class StackMapTableAttribute : public Attribute
    {
        // TODO:
    };

    class ExceptionAttribute : public Attribute
    {
        // TODO:
    };

    class InnerClassesAttribute : public Attribute
    {
        // TODO:
    };

    class EnclosingMethodAttribute : public Attribute
    {
        // TODO:
    };

    class SyntheticAttribute : public Attribute
    {
        // TODO:
    };

    class SignatureAttribute : public Attribute
    {
        // TODO:
    };

    class SourceFileAttribute : public Attribute
    {
    private:
        u2 source_file_index_;

    public:
        SourceFileAttribute(u2 attribute_name_index, u2 source_file_index)
          : Attribute(attribute_name_index)
          , source_file_index_(source_file_index)
        {}

        void
        jasm(std::ostream &os, const ConstantPool *pool = nullptr) const override;

        void
        emit_bytecode(std::ostream &os) const override;

        inline u4
        length() const override
        {
            return 2;
        }
    };

    class SourceDebugExtensionAttribute : public Attribute
    {
        // TODO:
    };

    class LineNumberTableAttribute : public Attribute
    {
    public:
        struct LineNumberTableEntry
        {
            u2 start_pc;
            u2 line_number;

            LineNumberTableEntry(u2 start_pc, u2 line_number)
              : start_pc(start_pc)
              , line_number(line_number)
            {}
        };

    private:
        std::vector<LineNumberTableEntry> line_number_table_;

    public:
        LineNumberTableAttribute(u2 attribute_name_index)
          : Attribute(attribute_name_index)
        {}

        inline void
        make_line_number_table_entry(u2 start_pc, u2 line_number)
        {
            line_number_table_.emplace_back(start_pc, line_number);
        }
    };

    class LocalVariableTableAttribute : public Attribute
    {
        // TODO:
    };

    class LocalVariableTypeTableAttribute : public Attribute
    {
        // TODO:
    };

    class DeprecatedAttribute : public Attribute
    {
        // TODO:
    };

    class RuntimeVisibleAnnotationsAttribute : public Attribute
    {
        // TODO:
    };

    class RuntimeInvisibleAnnotationsAttribute : public Attribute
    {
        // TODO:
    };

    class RuntimeVisibleParameterAnnotationsAttribute : public Attribute
    {
        // TODO:
    };

    class RuntimeInvisibleParameterAnnotationsAttribute : public Attribute
    {
        // TODO:
    };

    class AnnotationsDefaultAttribute : public Attribute
    {
        // TODO:
    };

    class BootstrapMethodsAttribute : public Attribute
    {
        // TODO:
    };

}

#endif // JAWA_ATTRIBUTE_HPP
