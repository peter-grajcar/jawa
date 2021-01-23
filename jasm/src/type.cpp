/**
 * @file type.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include <sstream>
#include <memory>

#include "type.hpp"
#include "byte_code/byte_code.hpp"

namespace jasm
{

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
        ss << element_type_->descriptor();
        return ss.str();
    }

    std::string BaseType::descriptor() const
    {
        return std::string(1, prefix());
    }

}