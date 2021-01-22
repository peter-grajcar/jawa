/**
 * @file type.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include <sstream>
#include "type.hpp"
#include "byte_code/byte_code.hpp"

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
        return byte_code::CLASS_TYPE_PREFIX;
    }

    std::string ClassType::descriptor() const
    {
        return prefix() + class_name_ + ";";
    }

    char ArrayType::prefix() const
    {
        return byte_code::ARRAY_TYPE_PREFIX;
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

    PrimitiveType BooleanType(byte_code::BOOLEAN_TYPE_PREFIX);
    PrimitiveType CharType(byte_code::CHAR_TYPE_PREFIX);
    PrimitiveType ShortType(byte_code::SHORT_TYPE_PREFIX);
    PrimitiveType IntType(byte_code::INTEGER_TYPE_PREFIX);
    PrimitiveType LongType(byte_code::LONG_TYPE_PREFIX);
    PrimitiveType FloatType(byte_code::FLOAT_TYPE_PREFIX);
    PrimitiveType DoubleType(byte_code::DOUBLE_TYPE_PREFIX);
}