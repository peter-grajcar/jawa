/**
 * @file type.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_TYPE_HPP
#define JAWA_TYPE_HPP

#include <string>
#include <vector>

#include "byte_code.hpp"

namespace jasm
{

    using namespace byte_code;

    class BaseType;

    template <char p>
    class PrimitiveType;

    class ReferenceType;

    class ClassType;

    class ArrayType;

    using Type = BaseType;

    using BooleanType = PrimitiveType<byte_code::BooleanTypePrefix>;
    using CharType = PrimitiveType<byte_code::CharTypePrefix>;
    using ByteType = PrimitiveType<byte_code::ByteTypePrefix>;
    using ShortType = PrimitiveType<byte_code::ShortTypePrefix>;
    using IntType = PrimitiveType<byte_code::IntTypePrefix>;
    using LongType = PrimitiveType<byte_code::LongTypePrefix>;
    using FloatType = PrimitiveType<byte_code::FloatTypePrefix>;
    using DoubleType = PrimitiveType<byte_code::DoubleTypePrefix>;
    using VoidType = PrimitiveType<byte_code::VoidTypePrefix>;

    /**
     * Abstract type class.
     */
    class BaseType
    {
    public:
        BaseType() = default;

        BaseType(BaseType &) = delete;

        BaseType(BaseType &&) = default;

        BaseType &operator=(const BaseType &) = delete;

        BaseType &operator=(BaseType &&) = default;

        virtual ~BaseType() = default;

        /**
         * Detrmines whether a type is a reference type.
         *
         * @return true for array ([) and class instance (L) types
         */
        virtual bool is_reference_type() const = 0;

        /**
         * Returns the type prefix which will be used in the byte code.
         *
         * @return type prefix.
         */
        virtual char prefix() const = 0;

        /**
         * Returns the type descriptor used in the byte code.
         *
         * @return type descriptor.
         */
        virtual utf8 descriptor() const;
    };

    template <char p>
    class PrimitiveType : public BaseType
    {
    public:
        explicit PrimitiveType() {}

        bool is_reference_type() const override
        {
            return true;
        }

        char prefix() const override
        {
            return p;
        }

        utf8 descriptor() const override
        {
            return std::string(1, p);
        }
    };

    class ReferenceType : public BaseType
    {
    public:
        virtual ~ReferenceType() = default;

        bool is_reference_type() const override;
    };

    class ClassType : public ReferenceType
    {
    private:
        std::string class_name_;
    public:
        explicit ClassType(const char *name) : class_name_(name) {}

        explicit ClassType(std::string &name) : class_name_(name) {}

        char prefix() const override;

        utf8 descriptor() const override;
    };

    class ArrayType : public ReferenceType
    {
    private:
        size_t dimension_;
        Type *element_type_;
    public:
        explicit ArrayType(Type *element_type, size_t dim = 1) : element_type_(element_type),
                                                                 dimension_(dim)
        {
            assert(element_type_ && dim != 0);
        }

        char prefix() const override;

        utf8 descriptor() const override;
    };

    class MethodSignatureType : public BaseType
    {
    private:
        Type *return_type_;
        std::vector<Type *> argument_types_;
    public:
        template <typename ...Args>
        MethodSignatureType(Type *return_type, Args ...argument_types) : return_type_(return_type),
                                                                         argument_types_{argument_types...}
        {
            assert(return_type_);
        }

        bool is_reference_type() const override;

        char prefix() const override;

        utf8 descriptor() const override;
    };

    // TODO: class VariableType

}

#endif //JAWA_TYPE_HPP
