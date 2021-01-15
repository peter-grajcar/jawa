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
%define parse.error custom


/* parser.hpp */
%code requires
{
#include <sstream>

#include "bisonflex.hpp"
#include "context.hpp"
#include "modifiers.hpp"
#include "types.hpp"
}

/* parser.cpp */
%code
{
YY_DECL;
}

%param {yyscan_t yyscanner}		// the name yyscanner is enforced by Flex
%param {jawa::context *ctx}

%start compilation_unit

%token EOF		        0               "koniec pliku"

/* modifiers */
%token                  PRIVATE         "prywatny/prywatna"
%token                  PROTECTED       "chroniony/chroniona"
%token<jawa::gender>    PUBLIC          "publiczny/publiczna"
%token<jawa::gender>    STATIC          "statyczny/statyczna"
%token                  FINAL           "końcowy/końcowa"
%token                  NATIVE          "ojczysty/ojczysta"
%token                  VOLATILE        "zmeinny/zmeinna"
%token                  SYNCHRONIZED    "zsynchronizowany/zsynchronizowana"
%token                  ABSTRACT        "abstrakcyjny/abstrakcyjna"
%token                  TRANSIENT       "przejściowy/przejściowa"

/* control flow */
%token                  IF              "jeśli"
%token                  ELSE            "albo"
%token                  FOR             "dla"
%token                  DO              "zrob"
%token                  WHILE           "dopóki"
%token                  RETURN          "zwróć"
%token                  TRY             "spróbuj"
%token                  CATCH           "łap"
%token                  FINALLY         "wreszcie"
%token                  CONTINUE        "kontyntynuj"
%token                  SWITCH          "przełącz"
%token                  DEFAULT         "domyślna"
%token                  BREAK           "złam"
%token                  CASE            "przypad"
%token                  THROW           "rzuć"

/* types */
%token                  VOID            "void"
%token                  INT             "całość"
%token                  SHORT           "krótki"
%token                  CHAR            "znak"
%token                  BYTE            "bajt"
%token                  LONG            "długy"
%token                  DOUBLE          "podwójny"
%token                  FLOAT           "pojedynczy"
%token                  BOOLEAN         "boolowski"

/* classes */
%token                  CLASS           "klasa"
%token                  INTERFACE       "urządzenie"
%token                  ENUM            "wyliczenie"
%token                  SUPER           "nadzbiór"
%token                  EXTENDS         "przedłuża"
%token                  IMPLEMENTS      "realizuje"
%token                  THIS            "to"
%token                  ASSERT          "potwierdzić"
%token                  PACKAGE         "pakiet"
%token                  IMPORT          "zaimportuj"

%token                  THROWS          "rzuca"

/* operators */
%token                  INSTANCEOF      "wystąpienie"
%token                  NEW             "nowy"
%token                  SEMIC           ";"
%token                  LCUR            "{"
%token                  RCUR            "}"
%token                  LPAR            "("
%token                  RPAR            ")"
%token                  LBRA            "["
%token                  RBRA            "]"
%token                  AMP             "&"
%token                  DAMP            "&&"
%token                  VERT            "|"
%token                  DVERT           "||"
%token                  HAT             "^"
%token                  TILDE           "~"
%token                  ASGN            "="
%token                  COLON           ":"
%token                  INTRG           "?"
%token                  ARROW           "->"
%token                  STAR            "*"
%token                  ADDOP           "+, -"
%token                  DIVOP           "/, %"
%token                  INCDEC          "++, --"
%token                  CMPE            "==, !="
%token                  SHIFT           "<<, >>, <<<"
%token                  CMP             "<, <=, >, >="
%token                  COMP            "+= , -=,  *=,  /=,  &=,  |=,  ^=,  %=,  <<=,  >>=,  >>>="

/* constants */
%token<jawa::Name>      IDF             "identyfikator"
%token<jawa::Name>      STR_LIT         "łancuch"
%token<jawa::int_t>     INT_LIT         "literał całkowity"
%token                  TRUE            "prawda"
%token                  FALSE           "nieprawda"

%%

compilation_unit: PUBLIC CLASS IDF LCUR PUBLIC STATIC VOID IDF
                  ;

%%

namespace jawa {

    void parser::report_syntax_error(parser::context const &parser_ctx) const
    {
        // Max 3 expected tokens
        symbol_kind_type expected[3];
        int n = parser_ctx.expected_tokens(expected, 3);

        std::stringstream msg;
        msg << "nieoczekiwany znak: " << symbol_name(parser_ctx.token()) << " oczekiwany:";
        for (int i = 0; i < n; ++i) {
            msg << ' ';
            msg << symbol_name(expected[i]);
            if (i + 1 < n)
                msg << " or";
        }

        ctx->message(error::SYNTAX, parser_ctx.location(), msg.str());
    }

    void parser::error(const location_type& loc, const std::string& msg)
    {
         ctx->message(error::SYNTAX, loc, msg);
    }

}

