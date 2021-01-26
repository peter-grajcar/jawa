/**
 * @file type.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_TYPE_HPP
#define JAWA_TYPE_HPP

#include <string>

#include "byte_code.hpp"

namespace jasm
{

    class BaseType;

    template <char p>
    class PrimitiveType;

    class ReferenceType;

    class ClassType;

    class ArrayType;

    using Type = BaseType;

    using BooleanType = PrimitiveType<byte_code::BOOLEAN_TYPE_PREFIX>;
    using CharType = PrimitiveType<byte_code::CHAR_TYPE_PREFIX>;
    using ByteType = PrimitiveType<byte_code::BYTE_TYPE_PREFIX>;
    using ShortType = PrimitiveType<byte_code::SHORT_TYPE_PREFIX>;
    using IntType = PrimitiveType<byte_code::INT_TYPE_PREFIX>;
    using LongType = PrimitiveType<byte_code::LONG_TYPE_PREFIX>;
    using FloatType = PrimitiveType<byte_code::FLOAT_TYPE_PREFIX>;
    using DoubleType = PrimitiveType<byte_code::DOUBLE_TYPE_PREFIX>;


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
        virtual std::string descriptor() const;
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

        std::string descriptor() const override
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

        std::string descriptor() const override;
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
            assert(dim != 0);
        }

        char prefix() const override;

        std::string descriptor() const override;
    };

    // TODO: class VariableType

}

#endif //JAWA_TYPE_HPP
