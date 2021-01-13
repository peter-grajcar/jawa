/**
 * @file context.hpp
 * Copyright (c) 2021 Peter Grajcar
 */

#ifndef JAWA_CONTEXT_HPP
#define JAWA_CONTEXT_HPP

namespace jawa {

    using loc_t = unsigned;

    /**
     * A class for storing the compiler context.
     */
    class context {
    private:
        loc_t loc;
    public:
        /**
         * Returns the line number.
         *
         * @return line number.
         */
        inline loc_t line() const
        {
            return loc;
        };

        /**
         * Increments the line number.
         */
        inline void inc_line()
        {
            ++loc;
        }
    };

}

#endif //JAWA_CONTEXT_HPP
