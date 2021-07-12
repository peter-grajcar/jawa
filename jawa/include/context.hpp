/**
 * @file Context.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONTEXT_HPP
#define JAWA_CONTEXT_HPP

#include <iostream>
#include <sstream>

#include "format.hpp"
#include "error.hpp"
#include "tables.hpp"
#include "builder.hpp"

namespace jawa
{

    extern std::ostringstream line_buffer;

    struct loc_t
    {
        loc_t() : line(1), column_start(1), column_end(1) {}

        unsigned line;
        unsigned column_start;
        unsigned column_end;
    };

    /**
     * A class for storing the compiler Context.
     */
    class Context
    {
    private:
        loc_t loc_;
        TypeTable type_table_;
        std::locale locale_;
        std::unique_ptr<jasm::ClassBuilder> builder_;

        void message_line(loc_t const &loc) const;

    public:
        Context() : locale_("pl_PL.UTF-8") {}

        /**
         * Returns current location of the parser.
         *
         * @return current location.
         */
        inline const loc_t &loc() const
        {
            return loc_;
        }

        /**
         * Returns used locale.
         *
         * @return locale.
         */
        inline const std::locale &locale() const
        {
            return locale_;
        }

        /**
         * Increments the line number.
         */
        inline void inc_line()
        {
            loc_.column_start = 1;
            loc_.column_end = 1;
            ++loc_.line;
        }

        /**
         * Increments the line number.
         */
        inline void inc_column(unsigned n)
        {
            loc_.column_start = loc_.column_end;
            loc_.column_end += n;
        }

        /**
         * Increments the line number.
         */
        inline void dec_column(unsigned n)
        {
            loc_.column_end = loc_.column_start;
            loc_.column_start = loc_.column_end - n;
        }

        /**
         * Reports error message.
         *
         * @param err type of the error.
         * @param loc location of the error.
         */
        template <typename ...Args>
        void message(errors::error_object<Args...> err, const loc_t &loc, Args... args) const
        {
            std::cerr << "błąd:" << loc.line << ':' << loc.column_start << ": ";
            format(std::cerr, err.msg(), args...) << std::endl;
            message_line(loc);
        }

        /**
         * Determines whether there are whitespaces between two tokens.
         *
         * @param lhs left hand side token.
         * @param rhs right hand side token.
         * @return true if there are no spaces in between the tokens, otherwise false.
         */
        bool no_spaces_between(const loc_t &lhs, const loc_t &rhs) const;

        /**
         * Determines whether a name corresponds to an existing type name.
         *
         * @param name the name of potential type.
         * @return true if the name corresponds to an existing type name, otherwise false.
         */
        bool is_type_name(const Name &name) const;

        /**
         * Returns current class builder.
         *
         * @return class builder.
         */
        inline jasm::ClassBuilder &class_builder()
        {
            return *builder_;
        }

        /**
         * Creates a new class builder.
         *
         * @param class_name name of a new class.
         * @return newly created class builder.
         */
        jasm::ClassBuilder &new_class_builder(Name class_name);

        /**
         *
         * @return reference to the type table.
         */
        inline TypeTable &type_table()
        {
            return type_table_;
        }

    };

    using context_t = Context *;

}

#endif //JAWA_CONTEXT_HPP
