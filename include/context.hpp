/**
 * @file context.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONTEXT_HPP
#define JAWA_CONTEXT_HPP

namespace jawa {

    struct loc_t
    {
        unsigned line;
        unsigned column;
    };

    /**
     * A class for storing the compiler context.
     */
    class context {
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

    };

}

#endif //JAWA_CONTEXT_HPP
