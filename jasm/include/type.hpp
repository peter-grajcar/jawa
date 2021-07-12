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
#include <utility>
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

        virtual size_t hash() const = 0;

        virtual bool operator==(const BaseType &type) const = 0;

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

        size_t hash() const override
        {
            return std::hash<char>{}(p);
        }

        bool operator==(const BaseType &type) const override
        {
            auto primitive_type = dynamic_cast<const PrimitiveType *>(&type);
            if (!primitive_type)
                return false;
            return primitive_type->prefix() == p;
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
        utf8 class_name_;
    public:
        explicit ClassType(const char *name) : class_name_(name) {}

        explicit ClassType(utf8 name) : class_name_(std::move(name)) {}

        char prefix() const override;

        utf8 descriptor() const override;

        size_t hash() const override
        {
            return std::hash<std::string>{}(class_name_);
        }

        bool operator==(const BaseType &type) const override;

        bool operator==(const ClassType &type) const;

    };

    class ArrayType : public ReferenceType
    {
    private:
        size_t dimension_;
        const Type *element_type_;
    public:
        explicit ArrayType(const Type *element_type, size_t dim = 1)
                : element_type_(element_type), dimension_(dim)
        {
            assert(element_type_ && dim != 0);
        }

        inline const Type *element_type() const
        {
            return element_type_;
        }

        char prefix() const override;

        utf8 descriptor() const override;

        size_t hash() const override
        {
            return element_type_->hash() ^ std::hash<size_t>{}(dimension_);
        }

        bool operator==(const BaseType &type) const override;

        bool operator==(const ArrayType &type) const;

    };

    class MethodSignatureType : public BaseType
    {
    private:
        const Type *return_type_;
        std::vector<const Type *> argument_types_;
    public:
        template <typename ...Args>
        MethodSignatureType(const Type *return_type, Args ...argument_types)
                : return_type_(return_type), argument_types_{argument_types...}
        {
            assert(return_type_);
        }

        MethodSignatureType(const Type *return_type, std::vector<const Type *> &&argument_types)
                : return_type_(return_type), argument_types_(argument_types)
        {
            assert(return_type_);
        }

        inline const Type *return_type() const
        {
            return return_type_;
        }

        inline const std::vector<const Type *> &argument_types() const
        {
            return argument_types_;
        }

        bool is_reference_type() const override;

        char prefix() const override;

        utf8 descriptor() const override;

        size_t hash() const override
        {
            size_t h = return_type_->hash();
            for (auto &arg_type : argument_types_)
                h ^= arg_type->hash();
            return h;
        }

        bool operator==(const BaseType &type) const override;

        bool operator==(const MethodSignatureType &type) const;

    };

    // TODO: class VariableType

}

#endif //JAWA_TYPE_HPP
