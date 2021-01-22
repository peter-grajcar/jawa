/**
 * @file type.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_TYPE_HPP
#define JAWA_TYPE_HPP

#include <string>

namespace jasm
{

    /**
     * Abstract type class.
     */
    class BaseType
    {
    public:
        /**
         * Detrmines whether a type is a reference type.
         *
         * @return true for array ([) and class instance (L) types
         */
        virtual bool is_reference_type() const = 0;

        virtual char prefix() const = 0;

        virtual std::string descriptor() const;
    };

    class PrimitiveType : BaseType
    {
    private:
        char prefix_;
    public:
        explicit PrimitiveType(char prefix) : prefix_(prefix) {}

        bool is_reference_type() const override;

        char prefix() const override;

        std::string descriptor() const override;
    };

    class ReferenceType : BaseType
    {
    public:
        bool is_reference_type() const override = 0;
    };

    class ClassType : ReferenceType
    {
    private:
        std::string class_name_;
    public:
        explicit ClassType(std::string &name) : class_name_(name) {}

        char prefix() const override;

        std::string descriptor() const override;
    };

    class ArrayType : ReferenceType
    {
    private:
        size_t dimension_;
        jasm::BaseType *type_;
    public:
        explicit ArrayType(jasm::BaseType *type, size_t dim = 1) : type_(type), dimension_(dim)
        {
            assert(dim != 0);
        }

        char prefix() const override;

        std::string descriptor() const override;
    };

    extern PrimitiveType BooleanType;
    extern PrimitiveType CharType;
    extern PrimitiveType ShortType;
    extern PrimitiveType IntType;
    extern PrimitiveType LongType;
    extern PrimitiveType FloatType;
    extern PrimitiveType DoubleType;

}

#endif //JAWA_TYPE_HPP
