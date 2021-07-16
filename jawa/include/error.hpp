/**
 * @file error.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
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

    extern err_n CLASS_NOT_FOUND;
    extern err_nn METHOD_NOT_FOUND;
    extern err_nn FIELD_NOT_FOUND;

}

#endif //JAWA_ERROR_HPP
