/**
 * @file error.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_ERROR_HPP
#define JAWA_ERROR_HPP

#include <string>
#include <array>

#include "types.hpp"

namespace jawa::errors
{

    template <typename ... Args>
    class error_object
    {
    private:
        const char *msg_;
    public:
        error_object(const char *msg) : msg_(msg) {}

        inline const char *msg() const
        {
            return msg_;
        }
    };

    using err = error_object<>;
    using err_c = error_object<char>;
    using err_s = error_object<char *>;
    using err_n = error_object<Name>;
    using err_nn = error_object<Name, Name>;

    extern err_s RESERVED;
    extern err_nn SYNTAX;
    extern err_n MALFORMED_STRING;
    extern err_c UNEXPECTED_CHAR;

}

#endif //JAWA_ERROR_HPP
