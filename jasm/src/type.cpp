/**
 * @file type.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#include <memory>
#include <sstream>

#include "byte_code.hpp"
#include "type.hpp"

namespace jasm {

    bool
    ReferenceType::is_reference_type() const
    {
        return true;
    }

    char
    ClassType::prefix() const
    {
        return byte_code::ClassTypePrefix;
    }

    utf8
    ClassType::descriptor() const
    {
        return prefix() + class_name_ + ";";
    }

    bool
    ClassType::operator==(const BaseType &type) const
    {
        auto class_type = dynamic_cast<const ClassType *>(&type);
        if (!class_type)
            return false;
        return *this == *class_type;
    }

    bool
    ClassType::operator==(const ClassType &class_type) const
    {
        return class_type.class_name_ == class_name_;
    }

    char
    ArrayType::prefix() const
    {
        return byte_code::ArrayTypePrefix;
    }

    utf8
    ArrayType::descriptor() const
    {
        std::ostringstream ss;
        for (size_t i = 0; i < dimension_; ++i)
            ss << prefix();
        ss << element_type_->descriptor();
        return ss.str();
    }

    bool
    ArrayType::operator==(const BaseType &type) const
    {
        auto array_type = dynamic_cast<const ArrayType *>(&type);
        if (!array_type)
            return false;
        return *this == *array_type;
    }

    bool
    ArrayType::operator==(const ArrayType &array_type) const
    {
        return array_type.element_type_ == element_type_ && array_type.dimension_ == dimension_;
    }

    utf8
    BaseType::descriptor() const
    {
        return std::string(1, prefix());
    }

    char
    MethodType::prefix() const
    {
        return '(';
    }

    utf8
    MethodType::descriptor() const
    {
        std::ostringstream ss;
        ss << '(';
        for (const Type *arg : argument_types_)
            ss << arg->descriptor();
        ss << ')';
        ss << return_type_->descriptor();
        return ss.str();
    }

    bool
    MethodType::is_reference_type() const
    {
        return false;
    }

    bool
    MethodType::operator==(const BaseType &type) const
    {
        auto method_type = dynamic_cast<const MethodType *>(&type);
        if (!method_type)
            return false;
        return *this == *method_type;
    }

    bool
    MethodType::operator==(const MethodType &method_type) const
    {
        if (method_type.return_type_ != return_type_)
            return false;
        if (method_type.argument_types_.size() != argument_types_.size())
            return false;
        for (auto it1 = method_type.argument_types_.begin(), it2 = argument_types_.begin(); it2 < argument_types_.end();
             ++it1, ++it2) {
            if (*it1 != *it2)
                return false;
        }
        return true;
    }
}