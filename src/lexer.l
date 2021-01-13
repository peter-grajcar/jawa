%{
#include "bisonflex.hpp"
#include "context.hpp"

#include "parser.hpp"
%}

%option noyywrap nounput noinput
%option batch never-interactive reentrant
%option nounistd


HEX_DIGIT       [0-9a-fA-F]
UNICODE_MARKER  u+
UNICODE_ESCAPE  \\{UNICODE_MARKER}{HEX_DIGIT}{4}

LINE_TERMINATOR \n|\r|\r\n
INPUT_CHAR      [^\n\r]
WHITESPACE      \x20|\x09|\x0c

DIGIT           [0-9]

%%

{WHITESPACE}        // Do nothing

{LINE_TERMINATOR}   ctx->inc_line();

;           return jawa::parser::make_SEMIC(ctx->loc());

<<EOF>>     return jawa::parser::make_EOF(ctx->loc());

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

