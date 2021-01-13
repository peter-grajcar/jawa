/**
 * @file bisonflex.hpp
 * Copyright (c) 2021 Peter Grajcar
 */
#ifndef JAWA_BISONFLEX_HPP
#define JAWA_BISONFLEX_HPP

#include "context.hpp"
#include <cstdio>

#define YY_DECL jawa::parser::symbol_type yylex(yyscan_t yyscanner, jawa::context *ctx)

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

namespace jawa {
    yyscan_t lexer_init(FILE * iff);
    void lexer_shutdown(yyscan_t scanner);
}

#endif //JAWA_BISONFLEX_HPP
