/**
 * @file context.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONTEXT_HPP
#define JAWA_CONTEXT_HPP

#include "error.hpp"
#include <iostream>

namespace jawa
{

    struct loc_t
    {
        loc_t() : line(1), column(1) {}

        unsigned line;
        unsigned column;
    };

    /**
     * A class for storing the compiler context.
     */
    class context
    {
    private:
        loc_t loc_;
    public:
        inline const loc_t &loc() const
        {
            return loc_;
        }

        /**
         * Returns the line number.
         *
         * @return line number.
         */
        inline unsigned line() const
        {
            return loc_.line;
        };

        /**
         * Returns the column number.
         *
         * @return column number.
         */
        inline unsigned column() const
        {
            return loc_.column;
        }

        /**
         * Increments the line number.
         */
        inline void inc_line()
        {
            loc_.column = 0;
            ++loc_.line;
        }


        /**
         * Increments the line number.
         */
        inline void inc_column(unsigned n)
        {
            loc_.column += n;
        }

        /**
         * Reports error message.
         *
         * @param err type of the error.
         * @param loc location of the error.
         */
        void message(error::err err, const loc_t &loc) const
        {
            std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                      << err.msg() << std::endl;
        }

        /**
         * Reports an error with character that caused the error.
         *
         * @param err type of the error.
         * @param loc location of the error.
         * @param c character that caused the error.
         */
        void message(error::err_c err, const loc_t &loc, char c) const
        {
            std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                      << err.msg() << '\'' << c << '\'' << std::endl;
        }

        /**
         * Reports an error with name that caused the error.
         *
         * @param err type of the error.
         * @param loc location of the error.
         * @param n name that caused the error.
         */
        void message(error::err_n err, const loc_t &loc, const Name &name) const
        {
            std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                      << err.msg() << '\"' << name << '\"' << std::endl;
        }

    };

}

#endif //JAWA_CONTEXT_HPP
