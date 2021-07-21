/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
%{
#include <sstream>
#include <queue>

#include "bisonflex.hpp"
#include "context.hpp"
#include "parser.hpp"
%}

%option noyywrap nounput noinput
%option batch never-interactive reentrant
%option nounistd

LETTER              [_$a-zA-Z\x7F-\xFF]
DIGIT               [0-9]
HEX_DIGIT           [0-9a-fA-F]
LETTER_OR_DIGIT     {LETTER}|{DIGIT}
IDENTIFIER          {LETTER}{LETTER_OR_DIGIT}*

UNICODE_MARKER      u+
UNICODE_ESCAPE      \\{UNICODE_MARKER}{HEX_DIGIT}{4}

LINE_TERMINATOR     \n|\r|\r\n
WHITESPACE          \x20|\x09|\x0c

BIN_PREFIX          0[Bb]
OCT_PREFIX          0
HEX_PREFIX          0[Xx]
LONG_SUFFIX         [Ll]
FLOAT_SUFFIX        [Ff]
DOUBLE_SUFFIX       [Dd]

STR_CHAR            [^\n\r\"]

%x STRLIT
%x COMMENT

%%
%{
std::ostringstream str_buffer;
%}

<INITIAL,COMMENT>{LINE_TERMINATOR}  ctx->inc_line();

{WHITESPACE}                /* Do nothing */

    /* Comments */

\/\/[^\n\r]*                /* Do nothing */

\/\*                        BEGIN(COMMENT);

<COMMENT>{
    \*\/                    BEGIN(INITIAL);
    .                       /* Do nothing */
}

    /* Keywords */

prywatn[ya]                 return jawa::parser::make_PRIVATE(jawa::get_form(yytext), ctx->loc());
chronion[ya]                return jawa::parser::make_PROTECTED(jawa::get_form(yytext), ctx->loc());
publiczn[ya]                return jawa::parser::make_PUBLIC(jawa::get_form(yytext), ctx->loc());
statyczn[ya]                return jawa::parser::make_STATIC(jawa::get_form(yytext), ctx->loc());
końcow[ya]                  return jawa::parser::make_FINAL(jawa::get_form(yytext), ctx->loc());
ojczyst[ya]                 return jawa::parser::make_NATIVE(jawa::get_form(yytext), ctx->loc());
zmeinn[ya]                  return jawa::parser::make_VOLATILE(jawa::get_form(yytext), ctx->loc());
zsynchronizowan[ya]         return jawa::parser::make_SYNCHRONIZED(jawa::get_form(yytext), ctx->loc());
abstrakcyjn[ya]             return jawa::parser::make_ABSTRACT(jawa::get_form(yytext), ctx->loc());
rzejściow[ya]               return jawa::parser::make_TRANSIENT(jawa::get_form(yytext), ctx->loc());
ścisłezp                    return jawa::parser::make_STRICTFP(ctx->loc());

jeżeli                      return jawa::parser::make_IF(ctx->loc());
albo                        return jawa::parser::make_ELSE(ctx->loc());
dla                         return jawa::parser::make_FOR(ctx->loc());
wykonaj                     return jawa::parser::make_DO(ctx->loc());
dopóki                      return jawa::parser::make_WHILE(ctx->loc());
zwróć                       return jawa::parser::make_RETURN(ctx->loc());
spróbuj                     return jawa::parser::make_TRY(ctx->loc());
łap                         return jawa::parser::make_CATCH(ctx->loc());
wreszcie                    return jawa::parser::make_FINALLY(ctx->loc());
kontyntynuj                 return jawa::parser::make_CONTINUE(ctx->loc());
przełącz                    return jawa::parser::make_SWITCH(ctx->loc());
domyślna                    return jawa::parser::make_DEFAULT(ctx->loc());
złam                        return jawa::parser::make_BREAK(ctx->loc());
przypad                     return jawa::parser::make_CASE(ctx->loc());
rzuć                        return jawa::parser::make_THROW(ctx->loc());

void                        return jawa::parser::make_VOID(ctx->loc());
całość                      return jawa::parser::make_INT(ctx->loc());
krótki                      return jawa::parser::make_SHORT(ctx->loc());
znak                        return jawa::parser::make_CHAR(ctx->loc());
bajt                        return jawa::parser::make_BYTE(ctx->loc());
długy                       return jawa::parser::make_LONG(ctx->loc());
podwójny                    return jawa::parser::make_DOUBLE(ctx->loc());
pojedynczy                  return jawa::parser::make_FLOAT(ctx->loc());
boolowski                   return jawa::parser::make_BOOLEAN(ctx->loc());

klasa                       return jawa::parser::make_CLASS(ctx->loc());
międzymordzie               return jawa::parser::make_INTERFACE(ctx->loc());
\@międzymordzie             return jawa::parser::make_AT_INTERFACE(ctx->loc());
wyliczenie                  return jawa::parser::make_ENUM(ctx->loc());
nadzbiór                    return jawa::parser::make_SUPER(ctx->loc());
przedłuża                   return jawa::parser::make_EXTENDS(ctx->loc());
realizuje                   return jawa::parser::make_IMPLEMENTS(ctx->loc());
to                          return jawa::parser::make_THIS(ctx->loc());
potwierdzić                 return jawa::parser::make_ASSERT(ctx->loc());
pakiet                      return jawa::parser::make_PACKAGE(ctx->loc());
zaimportuj                  return jawa::parser::make_IMPORT(ctx->loc());

rzuca                       return jawa::parser::make_THROWS(ctx->loc());

prawda                      return jawa::parser::make_TRUE(ctx->loc());
nieprawda                   return jawa::parser::make_FALSE(ctx->loc());

wystąpienie                 return jawa::parser::make_INSTANCEOF(ctx->loc());
nowy                        return jawa::parser::make_NEW(ctx->loc());

    /* Reserved Keywords */

kurwa                       ctx->message(jawa::errors::RESERVED, ctx->loc(), yytext);

    /* Operators */

==                          return jawa::parser::make_EQOP(jawa::operators::EQ, ctx->loc());
!=                          return jawa::parser::make_EQOP(jawa::operators::NE, ctx->loc());

\<=                         return jawa::parser::make_RELOP(jawa::operators::LTE, ctx->loc());
>=                          return jawa::parser::make_RELOP(jawa::operators::GTE, ctx->loc());

\+=                         return jawa::parser::make_COMP(jawa::operators::COMP_ADD, ctx->loc());
-=                          return jawa::parser::make_COMP(jawa::operators::COMP_SUB, ctx->loc());
\*=                         return jawa::parser::make_COMP(jawa::operators::COMP_MUL, ctx->loc());
\/=                         return jawa::parser::make_COMP(jawa::operators::COMP_DIV, ctx->loc());
\%=                         return jawa::parser::make_COMP(jawa::operators::COMP_MOD, ctx->loc());
\<\<=                       return jawa::parser::make_COMP(jawa::operators::COMP_SHL, ctx->loc());
>>>=                        return jawa::parser::make_COMP(jawa::operators::COMP_SHR_U, ctx->loc());
>>=                         return jawa::parser::make_COMP(jawa::operators::COMP_SHR, ctx->loc());
&=                          return jawa::parser::make_COMP(jawa::operators::COMP_AND, ctx->loc());
\|=                         return jawa::parser::make_COMP(jawa::operators::COMP_OR, ctx->loc());
\^=                         return jawa::parser::make_COMP(jawa::operators::COMP_XOR, ctx->loc());
~=                          return jawa::parser::make_COMP(jawa::operators::COMP_NOT, ctx->loc());

\+\+                        return jawa::parser::make_INCDEC(jawa::operators::INC, ctx->loc());
--                          return jawa::parser::make_INCDEC(jawa::operators::DEC, ctx->loc());

\+                          return jawa::parser::make_ADDOP(jawa::operators::ADD, ctx->loc());
-                           return jawa::parser::make_ADDOP(jawa::operators::SUB, ctx->loc());

\/                          return jawa::parser::make_DIVOP(jawa::operators::DIV, ctx->loc());
\%                          return jawa::parser::make_DIVOP(jawa::operators::MOD, ctx->loc());

>>>                         return jawa::parser::make_USHR(ctx->loc());
>>                          return jawa::parser::make_SHR(ctx->loc());
\<\<                        return jawa::parser::make_SHL(ctx->loc());

\.\.\.                      return jawa::parser::make_DOTS(ctx->loc());
;                           return jawa::parser::make_SEMIC(ctx->loc());
,                           return jawa::parser::make_COMMA(ctx->loc());
\<                          return jawa::parser::make_LT(ctx->loc());
>                           return jawa::parser::make_GT(ctx->loc());
\{                          return jawa::parser::make_LCUR(ctx->loc());
\}                          return jawa::parser::make_RCUR(ctx->loc());
\(                          return jawa::parser::make_LPAR(ctx->loc());
\)                          return jawa::parser::make_RPAR(ctx->loc());
\[                          return jawa::parser::make_LBRA(ctx->loc());
\]                          return jawa::parser::make_RBRA(ctx->loc());
&&                          return jawa::parser::make_DAMP(ctx->loc());
&                           return jawa::parser::make_AMP(ctx->loc());
\|\|                        return jawa::parser::make_DVERT(ctx->loc());
\|                          return jawa::parser::make_VERT(ctx->loc());
\^                          return jawa::parser::make_HAT(ctx->loc());
~                           return jawa::parser::make_TILDE(ctx->loc());
=                           return jawa::parser::make_ASGN(ctx->loc());
:                           return jawa::parser::make_COLON(ctx->loc());
\?                          return jawa::parser::make_INTRG(ctx->loc());
\!                          return jawa::parser::make_EMPH(ctx->loc());
->                          return jawa::parser::make_ARROW(ctx->loc());
\*                          return jawa::parser::make_STAR(ctx->loc());
\@                          return jawa::parser::make_AT(ctx->loc());
\.                          return jawa::parser::make_DOT(ctx->loc());

    /* Names */

{IDENTIFIER}                return jawa::parser::make_IDF(yytext, ctx->loc());

    /* Strings */

\"                          {
                                BEGIN(STRLIT);
                                str_buffer.str("");
                            }

<STRLIT>{
    {STR_CHAR}+             str_buffer << yytext;

    \"                      {
                                    BEGIN(INITIAL);
                                    return jawa::parser::make_STR_LIT(str_buffer.str(), ctx->loc());
                            }

    .                       ctx->message(jawa::errors::MALFORMED_STRING, ctx->loc(), str_buffer.str());
}

    /* Numbers */

    /*
    {BIN_PREFIX}{DIGIT}+{LONG_SUFFIX}
    {OCT_PREFIX}{DIGIT}+{LONG_SUFFIX}
    {HEX_PREFIX}{DIGIT}+{LONG_SUFFIX}
    {BIN_PREFIX}{DIGIT}+
    {OCT_PREFIX}{DIGIT}+
    {HEX_PREFIX}{DIGIT}+
    {DIGIT}+{LONG_SUFFIX}
    */
                            /* TODO: replace std::stoi and chceck for overflow */
{DIGIT}+                    return jawa::parser::make_INT_LIT(std::stoi(yytext), ctx->loc());


    /* Other */

<<EOF>>                     return jawa::parser::make_EOF(ctx->loc());

.                           ctx->message(jawa::errors::UNEXPECTED_CHAR, ctx->loc(), yytext[0]);

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

