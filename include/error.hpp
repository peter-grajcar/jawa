/**
 * @file error.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_ERROR_HPP
#define JAWA_ERROR_HPP

#include "tables.hpp"
#include <array>

namespace jawa::errors
{

    template <typename ... Args>
    class error_object
    {
    private:
        const char *name_;
        const char *msg_;
    public:
        error_object(const char *name, const char *msg) : name_(name), msg_(msg) {}

        inline const char *name() const
        {
            return name_;
        }

        inline const char *msg() const
        {
            return msg_;
        }
    };

    using err = error_object<>;
    using err_n = error_object<Name>;
    using err_c = error_object<char>;

    extern err_n SYNTAX;
    extern err_c UNEXPECTED_CHAR;

}

#endif //JAWA_ERROR_HPP
