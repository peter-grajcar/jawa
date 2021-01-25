/**
 * @file field.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_FIELD_HPP
#define JAWA_FIELD_HPP

#include "attribute.hpp"

namespace jasm
{

    class Field : public Attributable
    {
    private:
        u2 access_flags_;
        u2 name_index_;
        u2 descriptor_index_;
    public:
        Field(u2 access_flags, u2 name_index, u2 descriptor_index)
                : access_flags_(access_flags), name_index_(name_index),
                  descriptor_index_(descriptor_index) {}

    };

}

#endif //JAWA_FIELD_HPP
