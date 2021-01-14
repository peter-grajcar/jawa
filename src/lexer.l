%{
#include "bisonflex.hpp"
#include "context.hpp"

#include "parser.hpp"
%}

%option noyywrap nounput noinput
%option batch never-interactive reentrant
%option nounistd


HEX_DIGIT           [0-9a-fA-F]
UNICODE_MARKER      u+
UNICODE_ESCAPE      \\{UNICODE_MARKER}{HEX_DIGIT}{4}

LINE_TERMINATOR     \n|\r|\r\n
WHITESPACE          \x20|\x09|\x0c

LETTER              [_$a-zA-Z\x7F-\xFF]
DIGIT               [0-9]
LETTER_OR_DIGIT     {LETTER}|{DIGIT}

%%

{WHITESPACE}        /* Do nothing */

{LINE_TERMINATOR}   ctx->inc_line();

    /* Keywords */

publiczn[ya]                return jawa::parser::make_PUBLIC(jawa::get_gender(yytext), ctx->loc());
statyczn[ya]                return jawa::parser::make_STATIC(jawa::get_gender(yytext), ctx->loc());
klasa                       return jawa::parser::make_CLASS(ctx->loc());
void                        return jawa::parser::make_VOID(ctx->loc());

;                           return jawa::parser::make_SEMIC(ctx->loc());
\{                          return jawa::parser::make_LCUR(ctx->loc());
\}                          return jawa::parser::make_RCUR(ctx->loc());
\(                          return jawa::parser::make_LPAR(ctx->loc());
\}                          return jawa::parser::make_RPAR(ctx->loc());
\[                          return jawa::parser::make_LBRA(ctx->loc());
\]                          return jawa::parser::make_RBRA(ctx->loc());

{LETTER}{LETTER_OR_DIGIT}*  return jawa::parser::make_IDF(yytext, ctx->loc());

<<EOF>>                     return jawa::parser::make_EOF(ctx->loc());

.                           ctx->message(jawa::error::UNEXPECTED_CHAR, ctx->loc(), yytext[0]);

%%

namespace jawa {

	yyscan_t lexer_init(FILE *iff)
	{
		yyscan_t scanner;
		yylex_init(&scanner);
		yyset_in(iff, scanner);
		return scanner;
	}

	void lexer_shutdown(yyscan_t scanner)
	{
		yyset_in(nullptr, scanner);
		yylex_destroy(scanner);
	}

}

