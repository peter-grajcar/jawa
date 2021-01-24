/**
 * @file attributes.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_ATTRIBUTES_HPP
#define JAWA_ATTRIBUTES_HPP

#include <vector>

namespace jasm
{

    class Attribute
    {
    public:
        virtual ~Attribute() = default;
    };

    class Attributable
    {
    protected:
        std::vector<std::unique_ptr<Attribute>> attributes_;
    public:
        virtual ~Attributable() = default;
    };

}

#endif //JAWA_ATTRIBUTES_HPP
