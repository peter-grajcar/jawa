/**
 * @file context.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONTEXT_HPP
#define JAWA_CONTEXT_HPP

#include <iostream>
#include <sstream>

#include "error.hpp"

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
        std::ostringstream line_buffer_;

        void message_line(loc_t const &loc) const;

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
            line_buffer_.str("");
        }


        /**
         * Increments the line number.
         */
        inline void inc_column(unsigned n)
        {
            loc_.column += n;
        }

        inline std::ostream &line_buffer()
        {
            return line_buffer_;
        }

        /**
         * Reports error message.
         *
         * @param err type of the error.
         * @param loc location of the error.
         */
        void message(error::err err, const loc_t &loc) const;

        /**
         * Reports an error with character that caused the error.
         *
         * @param err type of the error.
         * @param loc location of the error.
         * @param c character that caused the error.
         */
        void message(error::err_c err, const loc_t &loc, char c) const;

        /**
         * Reports an error with name that caused the error.
         *
         * @param err type of the error.
         * @param loc location of the error.
         * @param n name that caused the error.
         */
        void message(error::err_n err, const loc_t &loc, const Name &name) const;

    };

}

#endif //JAWA_CONTEXT_HPP
