/**
 * @file Context.cpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
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

    void Context::message_line(loc_t const &loc) const
    {
        std::cerr << ' ' << std::setw(5) << std::setfill(' ') << loc.line << " | "
                  << jawa::line_buffer.str() << std::endl
                  << "       | ";
        // column starts at 1, so subtracting 1 is safe
        for (unsigned i = 0; i < loc.column_start - 1; ++i) {
            std::cerr << ' ';
        }

        std::cerr << '^';

        // underline erroneous token
        for (unsigned i = loc.column_start + 1; i < loc.column_end; ++i) {
            std::cerr << '~';
        }

        std::cerr << std::endl;
    }

    bool Context::no_spaces_between(const loc_t &lhs, const loc_t &rhs) const
    {
        return lhs.column_end == rhs.column_start;
    }

    bool Context::is_type_name(const Name &name) const
    {
        // TODO: implement this method
        return name == "Łańcuch" || name == "System";
    }

    jasm::ClassBuilder &Context::new_class_builder(Name class_name)
    {
        builder_ = std::make_unique<jasm::ClassBuilder>(class_name);
        return *builder_;

    }

}
