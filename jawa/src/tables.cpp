/**
 * @file tables.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) 2021 Peter Grajcar
 */

#include "tables.hpp"

namespace jawa
{

    size_t JawaMethod::hash() const
    {
        return std::hash<std::string>()(signature.name) ^ signature.type->hash();
    }

    bool JawaMethod::operator==(const JawaMethod &field) const
    {
        return signature.name == field.signature.name && signature.type == field.signature.type;
    }


    size_t JawaField::hash() const
    {
        return std::hash<std::string>()(name) ^ type->hash();
    }

    bool JawaField::operator==(const JawaField &field) const
    {
        return name == field.name && type == field.type;
    }

    size_t JawaClass::hash() const
    {
        return std::hash<std::string>()(name_);
    }

    bool JawaClass::operator==(const JawaClass &clazz) const
    {
        return name_ == clazz.name_;
    }

    void ClassTable::load_class(const Name &fully_qualified_name)
    {
        // TODO
    }

}