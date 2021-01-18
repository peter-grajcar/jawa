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
#include "operators.hpp"
}

/* parser.cpp */
%code
{
YY_DECL;

using namespace jawa;
}

%param {yyscan_t yyscanner}		// the name yyscanner is enforced by Flex
%param {jawa::context *ctx}

%start CompilationUnit

%token                      EOF     0       "koniec pliku"

/* modifiers */
%token<gender>              PRIVATE         "prywatny/prywatna"
%token<gender>              PROTECTED       "chroniony/chroniona"
%token<gender>              PUBLIC          "publiczny/publiczna"
%token<gender>              STATIC          "statyczny/statyczna"
%token<gender>              FINAL           "końcowy/końcowa"
%token<gender>              NATIVE          "ojczysty/ojczysta"
%token<gender>              VOLATILE        "zmeinny/zmeinna"
%token<gender>              SYNCHRONIZED    "zsynchronizowany/zsynchronizowana"
%token<gender>              ABSTRACT        "abstrakcyjny/abstrakcyjna"
%token<gender>              TRANSIENT       "przejściowy/przejściowa"
%token                      STRICTFP        "???"

/* control flow */
%token                      IF              "jeśli"
%token                      ELSE            "albo"
%token                      FOR             "dla"
%token                      DO              "zrob"
%token                      WHILE           "dopóki"
%token                      RETURN          "zwróć"
%token                      TRY             "spróbuj"
%token                      CATCH           "łap"
%token                      FINALLY         "wreszcie"
%token                      CONTINUE        "kontyntynuj"
%token                      SWITCH          "przełącz"
%token                      DEFAULT         "domyślna"
%token                      BREAK           "złam"
%token                      CASE            "przypad"
%token                      THROW           "rzuć"

/* types */
%token                      VOID            "void"
%token                      INT             "całość"
%token                      SHORT           "krótki"
%token                      CHAR            "znak"
%token                      BYTE            "bajt"
%token                      LONG            "długy"
%token                      DOUBLE          "podwójny"
%token                      FLOAT           "pojedynczy"
%token                      BOOLEAN         "boolowski"

/* classes */
%token                      CLASS           "klasa"
%token                      INTERFACE       "urządzenie"
%token                      ENUM            "wyliczenie"
%token                      SUPER           "nadzbiór"
%token                      EXTENDS         "przedłuża"
%token                      IMPLEMENTS      "realizuje"
%token                      THIS            "to"
%token                      ASSERT          "potwierdzić"
%token                      PACKAGE         "pakiet"
%token                      IMPORT          "zaimportuj"

%token                      THROWS          "rzuca"

/* operators */
%token                      INSTANCEOF      "wystąpienie"
%token                      NEW             "nowy"
%token                      LCUR            "{"
%token                      RCUR            "}"
%token                      LPAR            "("
%token                      RPAR            ")"
%token                      LBRA            "["
%token                      RBRA            "]"
%token                      SEMIC           ";"
%token                      COMMA           ","
%token                      DOT             "."
%token                      DOTS            "..."
%token                      AT              "@"
%token                      AMP             "&"
%token                      DAMP            "&&"
%token                      VERT            "|"
%token                      DVERT           "||"
%token                      HAT             "^"
%token                      TILDE           "~"
%token                      ASGN            "="
%token                      COLON           ":"
%token                      INTRG           "?"
%token                      EMPH            "!"
%token                      ARROW           "->"
%token                      STAR            "*"
%token                      LT              "<"
%token                      GT              ">"
%token<operators::addop>    ADDOP           "+, -"
%token<operators::divop>    DIVOP           "/, %"
%token<operators::incdec>   INCDEC          "++, --"
%token<operators::eqop>     EQOP            "==, !="
%token<operators::shift>    SHIFT           "<<, >>, >>>"
%token<operators::relop>    RELOP           "<=, >="
%token<operators::comp>     COMP            "+= , -=,  *=,  /=,  &=,  |=,  ^=,  %=,  <<=,  >>=,  >>>="

/* constants */
%token<Name>                STR_LIT         "łancuch"
%token<byte_t>              BYTE_LIT        "literał bajtowy"
%token<short_t>             SHORT_LIT       "literał krótky"
%token<int_t>               INT_LIT         "literał całkowity"
%token<long_t>              LONG_LIT        "literał dlugy"
%token<char_t>              CHAR_LIT        "literał znakowy"
%token<float_t>             FLOAT_LIT       "literał pojedynczy"
%token<double_t>            DOUBLE_LIT      "literał podwójny"
%token                      TRUE            "prawda"
%token                      FALSE           "nieprawda"
%token                      NULL            "nula"

%token<Name>                IDENTIFIER          "identyfikator"
%token<Name>                TYPE_VARIABLE       "todo"
%token<Name>                PACKAGE_NAME        "todo"
%token<Name>                TYPE_NAME           "todo"
%token<Name>                EXPRESSION_NAME     "todo"
%token<Name>                METHOD_NAME         "todo"
%token<Name>                CLASS_NAME          "todo"

%%

CompilationUnit: Block
               ;

/* Types */

Type: PrimitiveType
    | ReferenceType
    ;

PrimitiveType: NumericType
             | BOOLEAN
             ;

NumericType: IntegralType
           | FloatingPointType
           ;

IntegralType: BYTE
            | SHORT
            | INT
            | LONG
            | CHAR
            ;

FloatingPointType: FLOAT
                 | DOUBLE
                 ;

ReferenceType: ClassOrInterfaceType
             | TYPE_VARIABLE
             | ArrayType
             ;

ClassOrInterfaceType: TYPE_NAME
                    ;

ClassType: ClassOrInterfaceType
         ;

InterfaceType: ClassOrInterfaceType
             ;

ArrayType: Type LBRA RBRA
         ;

/* Declarations */

Modifiers_opt: %empty
             | Modifiers
             ;

Modifiers: Modifier
         | Modifiers Modifier
         ;

Modifier: Annotation
        | PUBLIC
        | PROTECTED
        | PRIVATE
        | ABSTRACT
        | STATIC
        | FINAL
        | STRICTFP
        | NATIVE
        | TRANSIENT
        | SYNCHRONIZED
        | VOLATILE
        ;

/* Annotations */

Annotations_opt: %empty
               | Annotations
               ;

Annotations: Annotation
           | Annotations Annotation
           ;

Annotation: AT TYPE_NAME LPAR ElementValuePairs_opt RPAR
          | AT IDENTIFIER
          | AT IDENTIFIER LPAR ElementValue RPAR
          ;

ElementValuePairs_opt: %empty
                     | ElementValuePairs
                     ;

ElementValuePairs: ElementValuePair
                 | ElementValuePairs COMMA ElementValuePair
                 ;

ElementValuePair: IDENTIFIER ASGN ElementValue
                ;

ElementValue: ConditionalExpression
            | Annotation
            | ElementValueArrayInitializer
            ;

ElementValueArrayInitializer: LCUR ElementValues COMMA RCUR
                            | LCUR ElementValues RCUR
                            | LCUR COMMA RCUR
                            | LCUR RCUR
                            ;

ElementValues: ElementValue
             | ElementValues COMMA ElementValue
             ;

ArrayInitializer: LCUR VariableInitializers COMMA RCUR
                | LCUR VariableInitializers RCUR
                | LCUR COMMA RCUR
                | LCUR RCUR
                ;

VariableInitializers: VariableInitializer
                    | VariableInitializers COMMA VariableInitializer
                    ;

VariableInitializer: Expression
                   | ArrayInitializer
                   ;

/* Statements */

Block: LCUR BlockStatements_opt RCUR
     ;

BlockStatements_opt: %empty
                   | BlockStatements
                   ;

BlockStatements: BlockStatement
               | BlockStatements BlockStatement
               ;

BlockStatement: LocalVariableDeclarationStatement
              | Statement
              ;

LocalVariableDeclarationStatement: LocalVariableDeclaration SEMIC
                                 ;

LocalVariableDeclaration: Modifiers Type VariableDeclarators
                        | Type VariableDeclarators
                        ;

VariableDeclarators: VariableDeclarator
                   | VariableDeclarators COMMA VariableDeclarator
                   ;

VariableDeclarator: VariableDeclaratorId
                  | VariableDeclaratorId ASGN VariableInitializer
                  ;

VariableDeclaratorId: IDENTIFIER
                    | VariableDeclaratorId LBRA RBRA
                    ;

Statement: StatementWithoutTrailingSubstatement
         | LabeledStatement
         | IfThenStatement
         | IfThenElseStatement
         | WhileStatement
         | ForStatement
         ;

StatementWithoutTrailingSubstatement: Block
                                    | EmptyStatement
                                    | ExpressionStatement
                                    | AssertStatement
                                    | SwitchStatement
                                    | DoStatement
                                    | BreakStatement
                                    | ContinueStatement
                                    | ReturnStatement
                                    | SynchronizedStatement
                                    | ThrowStatement
                                    | TryStatement
                                    ;

StatementNoShortIf: StatementWithoutTrailingSubstatement
                  | LabeledStatementNoShortIf
                  | IfThenElseStatementNoShortIf
                  | WhileStatementNoShortIf
                  | ForStatementNoShortIf
                  ;

EmptyStatement: SEMIC
              ;

IfThenStatement: IF LPAR Expression RPAR Statement
               ;

IfThenElseStatement: IF LPAR Expression RPAR StatementNoShortIf ELSE Statement
                   ;

IfThenElseStatementNoShortIf: IF LPAR Expression RPAR StatementNoShortIf ELSE StatementNoShortIf
                            ;

LabeledStatement: IDENTIFIER COLON Statement
                ;

LabeledStatementNoShortIf: IDENTIFIER COLON StatementNoShortIf
                         ;

ExpressionStatement: StatementExpression SEMIC
                   ;

StatementExpression: Assignment
                   | PreIncDecExpression
                   | PostIncDecExpression
                   | MethodInvocation
                   ;

AssertStatement: ASSERT Expression SEMIC
               | ASSERT Expression COLON Expression SEMIC
               ;

SwitchStatement: SWITCH LPAR Expression RPAR SwitchBlock
               ;

SwitchBlock: LCUR SwitchBlockStatementGroups SwitchLabels RCUR
           | LCUR SwitchBlockStatementGroups RCUR
           | LCUR SwitchLabels RCUR
           | LCUR RCUR
           ;

SwitchBlockStatementGroups: SwitchBlockStatementGroup
                          | SwitchBlockStatementGroups SwitchBlockStatementGroup
                          ;

SwitchBlockStatementGroup: SwitchLabels BlockStatements
                         ;

SwitchLabels: SwitchLabel
            | SwitchLabels SwitchLabel
            ;

SwitchLabel: CASE ConstantExpression COLON
           | CASE EnumConstantName COLON
           | DEFAULT COLON
           ;

EnumConstantName: IDENTIFIER
                ;

WhileStatement: WHILE LPAR Expression RPAR Statement
              ;

WhileStatementNoShortIf: WHILE LPAR Expression RPAR StatementNoShortIf
                       ;

DoStatement: DO Statement WHILE LPAR Expression RPAR SEMIC
           ;

ForStatement: BasicForStatement
            ;

BasicForStatement: FOR LPAR ForInit_opt SEMIC Expression_opt SEMIC ForUpdate_opt RPAR Statement
                 ;

ForStatementNoShortIf: FOR LPAR ForInit_opt SEMIC Expression_opt SEMIC ForUpdate_opt RPAR StatementNoShortIf
                     ;

ForInit_opt: %empty
           | ForInit
           ;

ForInit: StatementExpressionList
       | LocalVariableDeclaration
       ;

ForUpdate_opt: %empty
             | ForUpdate
             ;

ForUpdate: StatementExpressionList
         ;

StatementExpressionList: StatementExpression
                       | StatementExpressionList COMMA StatementExpression
                       ;

IDENTIFIER_opt: %empty
              | IDENTIFIER
              ;

BreakStatement: BREAK IDENTIFIER_opt SEMIC
              ;

ContinueStatement: CONTINUE IDENTIFIER_opt SEMIC
                 ;

ReturnStatement: RETURN Expression_opt SEMIC
               ;

ThrowStatement: THROW Expression SEMIC

SynchronizedStatement: SYNCHRONIZED LPAR Expression RPAR Block
                     ;

TryStatement: TRY Block Catches
            | TRY Block Catches_opt Finally
            | TryWithResourcesStatement
            ;

Catches_opt: %empty
           | Catches
           ;

Catches: CatchClause
       | Catches CatchClause
       ;

CatchClause: CATCH LPAR CatchFormalParameter RPAR Block
           ;

CatchFormalParameter: Modifiers_opt CatchType VariableDeclaratorId
                    ;

CatchType: ClassType
         | ClassType VERT CatchType
         ;

Finally: FINALLY Block
       ;

TryWithResourcesStatement: TRY ResourceSpecification Block Catches Finally
                         | TRY ResourceSpecification Block Catches
                         | TRY ResourceSpecification Block Finally
                         | TRY ResourceSpecification Block
                         ;

SEMIC_opt: %empty
         | SEMIC
         ;

ResourceSpecification: LPAR Resources SEMIC_opt RPAR
                     ;

Resources: Resource
         | Resources SEMIC Resource
         ;

Resource: Modifiers_opt Type VariableDeclaratorId ASGN Expression
        ;

/* Expressions */

Primary: PrimaryNoNewArray
       | ArrayCreationExpression
       ;

PrimaryNoNewArray: Literal
                 | Type DOT CLASS
                 | VOID DOT CLASS
                 | THIS
                 | CLASS_NAME DOT THIS
                 | LPAR Expression RPAR
                 | FieldAccess
                 | MethodInvocation
                 | ArrayAccess
                 ;

Literal: INT_LIT
       | LONG_LIT
       | SHORT_LIT
       | BYTE_LIT
       | FLOAT_LIT
       | DOUBLE_LIT
       | CHAR_LIT
       | STR_LIT
       | TRUE
       | FALSE
       | NULL
       ;

ArgumentList_opt: %empty
                | ArgumentList
                ;

ArgumentList: Expression
            | ArgumentList COMMA Expression
            ;

ArrayCreationExpression: NEW PrimitiveType DimExprs Dims_opt
                       | NEW ClassOrInterfaceType DimExprs Dims_opt
                       | NEW PrimitiveType Dims ArrayInitializer
                       | NEW ClassOrInterfaceType Dims ArrayInitializer

DimExprs: DimExpr
        | DimExprs DimExpr
        ;

DimExpr: LBRA Expression RBRA
       ;

Dims_opt: %empty
        | Dims
        ;

Dims: LBRA RBRA
    | Dims LBRA RBRA
    ;

FieldAccess: Primary DOT IDENTIFIER
           | SUPER DOT IDENTIFIER
           | CLASS_NAME DOT SUPER DOT IDENTIFIER
           ;

MethodInvocation: METHOD_NAME LPAR ArgumentList_opt RPAR
                ;

ArrayAccess: EXPRESSION_NAME LBRA Expression RBRA
           | PrimaryNoNewArray LBRA Expression RBRA
           ;

PostfixExpression: Primary
                 | EXPRESSION_NAME
                 | PostIncDecExpression
                 ;

PostIncDecExpression: PostfixExpression INCDEC
                    ;

UnaryExpression: PreIncDecExpression
               | ADDOP UnaryExpression
               | UnaryExpressionNotPlusMinus
               ;

PreIncDecExpression: INCDEC UnaryExpression
                   ;

UnaryExpressionNotPlusMinus: PostfixExpression
                           | TILDE UnaryExpression
                           | EMPH UnaryExpression
                           | CastExpression
                           ;

CastExpression: LPAR PrimitiveType RPAR UnaryExpression
              | LPAR ReferenceType RPAR UnaryExpressionNotPlusMinus
              ;

MultiplicativeExpression: UnaryExpression
                        | MultiplicativeExpression STAR UnaryExpression
                        | MultiplicativeExpression DIVOP UnaryExpression
                        ;

AdditiveExpression: MultiplicativeExpression
                  | AdditiveExpression ADDOP MultiplicativeExpression
                  ;

ShiftExpression: AdditiveExpression
               | ShiftExpression SHIFT AdditiveExpression
               ;

RelationalExpression: ShiftExpression
                    | RelationalExpression LT ShiftExpression
                    | RelationalExpression GT ShiftExpression
                    | RelationalExpression RELOP ShiftExpression
                    | RelationalExpression INSTANCEOF ReferenceType
                    ;

EqualityExpression: RelationalExpression
                  | EqualityExpression EQOP RelationalExpression
                  ;

AndExpression: EqualityExpression
             | AndExpression AMP EqualityExpression
             ;

ExclusiveOrExpression: AndExpression
                     | ExclusiveOrExpression HAT AndExpression
                     ;

InclusiveOrExpression: ExclusiveOrExpression
                     | InclusiveOrExpression VERT ExclusiveOrExpression
                     ;

ConditionalAndExpression: InclusiveOrExpression
                        | ConditionalAndExpression DAMP InclusiveOrExpression
                        ;

ConditionalOrExpression: ConditionalAndExpression
                       | ConditionalOrExpression DVERT ConditionalAndExpression
                       ;

ConditionalExpression: ConditionalOrExpression
                     | ConditionalOrExpression INTRG Expression COLON ConditionalExpression
                     ;

AssignmentExpression: ConditionalExpression
                    | Assignment
                    ;

Assignment: LeftHandSide AssignmentOperator AssignmentExpression
          ;

LeftHandSide: EXPRESSION_NAME
            | FieldAccess
            | ArrayAccess
            ;

AssignmentOperator: ASGN
                  | COMP
                  ;

Expression_opt: %empty
              | Expression
              ;

Expression: AssignmentExpression
          ;

ConstantExpression: Expression
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

        ctx->message(errors::SYNTAX, parser_ctx.location(), msg.str());
    }

    void parser::error(const location_type& loc, const std::string& msg)
    {
         ctx->message(errors::SYNTAX, loc, msg);
    }

}

