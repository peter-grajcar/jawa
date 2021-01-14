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
}

/* parser.cpp */
%code
{
YY_DECL;
}

%param {yyscan_t yyscanner}		// the name yyscanner is enforced by Flex
%param {jawa::context *ctx}

%start compilation_unit

%token EOF		            0   "end of file"
%token SEMIC                    ";"

%token<jawa::gender> PUBLIC     "publiczny/publiczna"
%token<jawa::gender> STATIC     "statyczny/statyczna"

%token ABSTRACT         "abstract"
%token CONTINUE         "continue"
%token FOR              "for"
%token NEW              "new"
%token SWITCH           "switch"
%token ASSERT           "assert"
%token DEFAULT          "default"
%token IF               "if"
%token PACKAGE          "package"
%token SYNCHRONIZED     "synchronizes"
%token BOOLEAN          "boolean"
%token DO               "do"
%token GOTO             "goto"
%token PRIVATE          "private"
%token THIS             "this"
%token BREAK            "break"
%token DOUBLE           "double"
%token IMPLEMENTS       "implements"
%token PROTECTED        "protected"
%token THROW            "throw"
%token BYTE             "byte"
%token ELSE             "else"
%token IMPORT           "import"
%token THROWS           "throws"
%token CASE             "case"
%token ENUM             "enum"
%token INSTANCEOF       "instanceof"
%token RETURN           "return"
%token TRANSIENT        "transient"
%token CATCH            "catch"
%token EXTENDS          "extends"
%token INT              "int"
%token SHORT            "short"
%token TRY              "try"
%token CHAR             "char"
%token FINAL            "final"
%token INTERFACE        "interface"
%token VOID             "void"
%token CLASS            "class"
%token FINALLY          "finally"
%token LONG             "long"
%token STRICTFP         "strictfp"
%token VOLATILE         "volatile"
%token CONST            "const"
%token FLOAT            "float"
%token NATIVE           "native"
%token SUPER            "super"
%token WHILE            "while"

%token LCUR             "{"
%token RCUR             "}"
%token LPAR             "("
%token RPAR             ")"
%token LBRA             "["
%token RBRA             "]"

%token<jawa::Name> IDF  "identyfikator"

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

