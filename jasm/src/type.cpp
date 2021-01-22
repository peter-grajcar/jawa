/**
 * @file type.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "type.hpp"
#include <sstream>

namespace jasm
{

    bool PrimitiveType::is_reference_type() const
    {
        return false;
    }

    char PrimitiveType::prefix() const
    {
        return prefix_;
    }

    bool ReferenceType::is_reference_type() const
    {
        return true;
    }

    char ClassType::prefix() const
    {
        return 'L';
    }

    std::string ClassType::descriptor() const
    {
        return prefix() + class_name_ + ";";
    }

    char ArrayType::prefix() const
    {
        return '[';
    }

    std::string ArrayType::descriptor() const
    {
        std::ostringstream ss;
        for (size_t i = 0; i < dimension_; ++i)
            ss << prefix();
        ss << type_->descriptor();
        return ss.str();
    }

    std::string BaseType::descriptor() const
    {
        return std::string(1, prefix());
    }

    std::string PrimitiveType::descriptor() const
    {
        return std::string(1, prefix());
    }

    PrimitiveType BooleanType('Z');
    PrimitiveType CharType('C');
    PrimitiveType ShortType('S');
    PrimitiveType IntType('I');
    PrimitiveType LongType('J');
    PrimitiveType FloatType('F');
    PrimitiveType DoubleType('D');
}