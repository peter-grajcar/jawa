%{
#include "bisonflex.hpp"
#include "context.hpp"

#include "parser.hpp"
%}

%option noyywrap nounput noinput
%option batch never-interactive reentrant
%option nounistd

%%

;           return jawa::parser::make_SEMIC(0);

<<EOF>>     return jawa::parser::make_EOF(0);

%%

namespace jawa {

	yyscan_t lexer_init(FILE * iff)
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

