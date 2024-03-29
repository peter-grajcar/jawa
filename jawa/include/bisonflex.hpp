/**
 * @file bisonflex.hpp
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
#ifndef JAWA_BISONFLEX_HPP
#define JAWA_BISONFLEX_HPP

#include <cstdio>

#include "context.hpp"
#include "unicode.hpp"

#define YY_DECL jawa::parser::symbol_type yylex(yyscan_t yyscanner, jawa::Context *ctx)

#define YY_USER_ACTION ctx->inc_column(jawa::unicode::utf8_length(yytext));

#define IGNORE_MATCHED                                                                                                 \
    ctx->dec_column(jawa::unicode::utf8_length(yytext));                                                               \
    yyless(0);

#define YYLLOC_DEFAULT(res, rhs, N) (res = (N) ? YYRHSLOC(rhs, 1) : YYRHSLOC(rhs, 0))

#define YY_INPUT(buf, result, max_size)                                                                                \
    jawa::line_buffer.str("");                                                                                         \
    int c = '*';                                                                                                       \
    int n;                                                                                                             \
    for (n = 0; n < max_size && (c = getc(yyin)) != EOF && c != '\n'; ++n) {                                           \
        buf[n] = (char) c;                                                                                             \
        jawa::line_buffer << (char) c;                                                                                 \
    }                                                                                                                  \
    if (c == '\n') {                                                                                                   \
        buf[n++] = (char) c;                                                                                           \
    }                                                                                                                  \
    if (c == EOF && ferror(yyin)) {                                                                                    \
        YY_FATAL_ERROR("input in flex scanner failed");                                                                \
    }                                                                                                                  \
    result = n;

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

namespace jawa {
    yyscan_t
    lexer_init(FILE *iff);

    void
    lexer_shutdown(yyscan_t scanner);
}

#endif // JAWA_BISONFLEX_HPP
