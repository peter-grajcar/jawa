/**
 * @file context.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "context.hpp"
#include <sstream>
#include <iomanip>

namespace jawa
{

    std::ostringstream line_buffer{};

    std::string escape(char ch)
    {
        std::ostringstream oss;
        if (ch < 32 || ch > 126 || ch == '\'' || ch == '"')
            oss << "\\x" << std::hex << std::setfill('0')
                << std::setw(2) << (int) ch;
        else
            oss.put(ch);
        return std::move(oss).str();

    }

    void context::message_line(loc_t const &loc) const
    {
        std::cerr << ' ' << std::setw(5) << loc.line << '|';
        std::cerr << jawa::line_buffer.str() << std::endl
                  << std::setw(7) << '|';
        // column starts at 1, so subtracting 1 is safe
        for (unsigned i = 0; i < loc.column_start - 1; ++i) {
            std::cerr << ' ';
        }

        // point to the erroneous single-character token
        if (loc.column_end - loc.column_start == 1) {
            std::cerr << '^' << std::endl;
            return;
        }

        // underline erroneous token
        for (unsigned i = loc.column_start; i < loc.column_end - 1; ++i) {
            std::cerr << '~';
        }
    }

    void context::message(errors::err err, const loc_t &loc) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column_start << ": "
                  << err.msg() << std::endl;
        message_line(loc);
    }

    void context::message(errors::err_c err, const loc_t &loc, char ch) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column_start << ": "
                  << err.msg() << '\'' << escape(ch) << '\'' << std::endl;
        message_line(loc);
    }

    void context::message(errors::err_n err, const loc_t &loc, const Name &name) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column_start << ": "
                  << err.msg() << '\"' << name << '\"' << std::endl;
        message_line(loc);
    }

    bool context::no_spaces_between(const loc_t &lhs, const loc_t &rhs) const
    {
        return lhs.column_end == rhs.column_start;
    }

}
