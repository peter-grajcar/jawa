/**
 * @file type.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <sstream>
#include <memory>

#include "type.hpp"
#include "byte_code.hpp"

namespace jasm
{

    bool ReferenceType::is_reference_type() const
    {
        return true;
    }

    char ClassType::prefix() const
    {
        return byte_code::ClassTypePrefix;
    }

    std::string ClassType::descriptor() const
    {
        return prefix() + class_name_ + ";";
    }

    char ArrayType::prefix() const
    {
        return byte_code::ArrayTypePrefix;
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