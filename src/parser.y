%language "c++"
%require "3.4"

%locations
%define api.location.type {jawa::loc_t}

%define api.namespace {jawa}
%define api.value.type variant
%define api.token.constructor
%define api.parser.class {parser}
%define api.token.prefix {TOKEN_}
%define parse.assert
%define parse.error verbose


/* parser.hpp */
%code requires
{
#include "bisonflex.hpp"
#include "context.hpp"

#define YYLLOC_DEFAULT(res,rhs,N)	(res = (N)?YYRHSLOC(rhs, 1):YYRHSLOC(rhs, 0))
}

/* parser.cpp */
%code
{
YY_DECL;
}

%param {yyscan_t yyscanner}		// the name yyscanner is enforced by Flex
%param {jawa::context *ctx}

%start translation_unit

%token EOF		0				"end of file"
%token SEMIC                    ";"

%%

translation_unit: SEMIC
                  ;

%%

namespace jawa {

	void parser::error(const location_type& l, const std::string& m)
	{
		// TODO:
	}

}

