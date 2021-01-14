/**
 * @file context.cpp
 * Copyright (c) 2021 Peter Grajcar
 */
#include "context.hpp"
#include <sstream>
#include <iomanip>

namespace jawa
{
    std::string escape(char ch)
    {
        std::ostringstream oss;
        if (ch < 32 || ch > 126 || ch == '\'' || ch == '"')
            oss << "\\x" << std::hex << std::setfill('0') << std::setw(2) << (int) ch;
        else
            oss.put(ch);
        return std::move(oss).str();

    }

    void context::message_line(loc_t const &loc) const
    {
        std::cerr << std::setw(4) << loc.line << ':';
        std::cerr << line_buffer_.str() << " ..." << std::endl
                  << std::setw(5) << ' ';
        // column starts at 1, so subtracting 1 is safe
        for (unsigned i = 0; i < loc.column - 1; ++i) {
            std::cerr << ' ';
        }
        std::cerr << '^' << std::endl;
    }

    void context::message(error::err err, const loc_t &loc) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                  << err.msg() << std::endl;
        message_line(loc);
    }

    void context::message(error::err_c err, const loc_t &loc, char ch) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                  << err.msg() << '\'' << escape(ch) << '\'' << std::endl;
        message_line(loc);
    }

    void context::message(error::err_n err, const loc_t &loc, const Name &name) const
    {
        std::cerr << "błąd:" << loc.line << ':' << loc.column << ": "
                  << err.msg() << '\"' << name << '\"' << std::endl;
        message_line(loc);
    }

}
