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
%token                      STRICTFP        "ścisłezp"

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
%token                      AT_INTERFACE    "@urządzenie"
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

%token<Name>                IDF             "identyfikator"


%%

/* Identifiers */

Identifier_opt: %empty | Identifier ;

Identifier: IDF
          ;

Name: Identifier
    | Name DOT Identifier
    ;

NameList: Name
        | NameList COMMA Name
        ;

/* Declarations */

CompilationUnit: PackageDeclaration_opt ImportDeclaration_opt TypeDeclaration_opt
               ;

PackageDeclaration_opt: %empty | PackageDeclaration ;

PackageDeclaration: PACKAGE Name SEMIC
                  ;

ImportDeclaration_opt: %empty | ImportDeclaration ;

ImportDeclaration: IMPORT Name SEMIC
                 ;

TypeDeclaration_opt: %empty | TypeDeclaration ;

TypeDeclaration: ClassOrInterfaceDeclaration
               | SEMIC
               ;

ClassOrInterfaceDeclaration: Modifiers_opt ClassDeclaration
                           | Modifiers_opt InterfaceDeclaration
                           ;

ClassDeclaration: NormalClassDeclaration
                | EnumDeclaration
                ;

InterfaceDeclaration: NormalInterfaceDeclaration
                    | AnnotationTypeDeclaration
                    ;

NormalClassDeclaration: CLASS Identifier TypeParameters_opt ClassExtends_opt Implements_opt ClassBody
                      ;

EnumDeclaration: ENUM Identifier Implements_opt EnumBody
               ;

NormalInterfaceDeclaration: INTERFACE Identifier TypeParameters_opt InterfaceExtends InterfaceBody
                          ;

AnnotationTypeDeclaration: AT_INTERFACE Identifier AnnotationTypeBody
                         ;

ClassExtends_opt: %empty | ClassExtends ;

ClassExtends: EXTENDS Type
            ;

InterfaceExtends_opt: %empty | InterfaceExtends ;

InterfaceExtends: EXTENDS TypeList
                ;

Implements_opt: %empty | Implements ;

Implements: IMPLEMENTS TypeList
          ;

/* Types */

Type: PrimitiveType
    | ReferenceType
    ;

TypeNoArguments: PrimitiveType
               | ReferenceTypeNoArguments
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
             | ArrayType
             ;

ReferenceTypeNoArguments: ClassOrInterfaceTypeNoArguments
                        | ArrayTypeNoArguments
                        ;

ClassOrInterfaceTypeNoArguments: Name
                               ;

ClassOrInterfaceType: Name
                    | TypeDeclSpecifier
                    ;

ClassType: TypeDeclSpecifier
         ;

InterfaceType: TypeDeclSpecifier
             ;

TypeDeclSpecifier: Name TypeArguments TypeDeclSpecifierTail;

TypeDeclSpecifierTail: DOT Name TypeArguments
                     | TypeDeclSpecifierTail DOT Name TypeArguments
                     ;

ArrayType: PrimitiveType Dims
         | Name Dims
         | TypeDeclSpecifier Dims
         ;

ArrayTypeNoArguments: PrimitiveType Dims
                    | ClassOrInterfaceTypeNoArguments Dims
                    ;

Dim_opt: %empty
       | Dim
       ;

Dim: LBRA RBRA
   ;

Dims_opt: %empty
        | Dims
        ;

Dims: Dim
    | Dims Dim
    ;

/* Type Arguments */

TypeArguments_opt: %empty | TypeArguments ;

TypeArguments: LT TypeArgumentList GT
             ;

TypeArgumentList: TypeArgument
                | TypeArgumentList COMMA TypeArgument
                ;

TypeArgument: ReferenceType
            | INTRG TypeArgumentBounds_opt
            ;

TypeArgumentBounds_opt: %empty | TypeArgumentBounds ;

TypeArgumentBounds: EXTENDS ReferenceType
                  | SUPER ReferenceType
                  ;

NonWildcardTypeArguments_opt: %empty | NonWildcardTypeArguments ;

NonWildcardTypeArguments: LT TypeList GT
                        ;

TypeList: ReferenceType
        | TypeList COMMA ReferenceType
        ;



TypeArgumentsOrDiamond_opt: %empty | TypeArgumentsOrDiamond ;

TypeArgumentsOrDiamond: LT GT
                      | TypeArguments
                      ;

NonWildcardTypeArgumentsOrDiamond_opt: %empty | NonWildcardTypeArgumentsOrDiamond ;

NonWildcardTypeArgumentsOrDiamond: LT GT
                                 | NonWildcardTypeArguments
                                 ;


TypeParameters_opt: %empty | TypeParameters ;

TypeParameters: LT TypeParameterList GT
              ;

TypeParameterList: TypeParameter
                 | TypeParameterList COMMA TypeParameter
                 ;

TypeParameter_opt: %empty | TypeParameter ;

TypeParameter: Identifier TypeParameterBound_opt
             ;

TypeParameterBound_opt: %empty | TypeParameterBound ;

TypeParameterBound: EXTENDS Bound
                  ;

Bound: ReferenceType
     | Bound AMP ReferenceType
     ;

Modifiers_opt: %empty | Modifiers ;

Modifiers: Modifier
         | Modifiers Modifier
         ;

Modifier: Annotation
        | PUBLIC
        | PROTECTED
        | PRIVATE
        | STATIC
        | ABSTRACT
        | FINAL
        | NATIVE
        | SYNCHRONIZED
        | TRANSIENT
        | VOLATILE
        | STRICTFP
        ;

Annotations_opt: %empty | Annotations ;

Annotations: Annotation
           | Annotations Annotation
           ;

Annotation: AT Name AnnotationParameters
          ;

AnnotationParameters: LPAR AnnotationElement_opt RPAR
                    ;

AnnotationElement_opt: %empty | AnnotationElement ;

AnnotationElement: ElementValuePairs
                 | ElementValue
                 ;

ElementValuePairs: ElementValuePair
                 | ElementValuePairs COMMA ElementValuePair
                 ;

ElementValuePair: Identifier ASGN ElementValue
                ;

ElementValue: Annotation
            | ConditionalExpression
            | ElementValueArrayInitializer
            ;

ElementValueArrayInitializer: LCUR RCUR
                            | LCUR COMMA RCUR
                            | LCUR ElementValues RCUR
                            | LCUR ElementValues COMMA RCUR
                            ;

ElementValues_opt: %empty | ElementValues ;

ElementValues: ElementValue
             | ElementValues COMMA ElementValue
             ;



ClassBody_opt: %empty | ClassBody ;

ClassBody: LCUR ClassBodyDeclarations_opt RCUR
         ;

ClassBodyDeclarations_opt: %empty | ClassBodyDeclarations ;

ClassBodyDeclarations: ClassBodyDeclaration
                     | ClassBodyDeclarations ClassBodyDeclaration
                     ;

ClassBodyDeclaration: SEMIC
                    | Modifiers_opt MemberDecl
                    | STATIC Block
                    ;

MemberDecl: MethodOrFieldDecl
          | VOID Identifier VoidMethodDeclaratorRest
          | Identifier ConstructorDeclaratorRest
          | GenericMethodOrConstructorDecl
          | ClassDeclaration
          | InterfaceDeclaration
          ;

MethodOrFieldDecl: Type Identifier MethodOrFieldRest
                 ;

MethodOrFieldRest: FieldDeclaratorsRest SEMIC
                 | MethodDeclaratorRest
                 ;

FieldDeclaratorsRest: VariableDeclarator
                    | VariableDeclaratorRest COMMA VariableDeclarator
                    ;

MethodDeclaratorRest: FormalParameters Dims_opt Throws_opt Block
                    | FormalParameters Dims_opt Throws_opt SEMIC
                    ;

Throws_opt: %empty | Throws ;

Throws: THROWS NameList
      ;

VoidMethodDeclaratorRest: FormalParameters Throws_opt Block
                        | FormalParameters Throws_opt SEMIC
                        ;

ConstructorDeclaratorRest: FormalParameters Throws_opt Block
                         ;

GenericMethodOrConstructorDecl: TypeParameters GenericMethodOrConstructorRest
                              ;

GenericMethodOrConstructorRest: Type Identifier MethodDeclaratorRest
                              | VOID Identifier MethodDeclaratorRest
                              | Identifier ConstructorDeclaratorRest
                              ;


InterfaceBody: LCUR InterfaceBodyDeclarations_opt RCUR
             ;

InterfaceBodyDeclarations_opt: %empty | InterfaceBodyDeclarations ;

InterfaceBodyDeclarations: InterfaceBodyDeclaration
                         | InterfaceBodyDeclarations InterfaceBodyDeclaration
                         ;

InterfaceBodyDeclaration: SEMIC
                        | Modifiers_opt InterfaceMemberDecl
                        ;

InterfaceMemberDecl: InterfaceMethodOrFieldDecl
                   | VOID Identifier VoidInterfaceMethodDeclaratorRest
                   | InterfaceGenericMethodDecl
                   | ClassDeclaration
                   | InterfaceDeclaration
                   ;

InterfaceMethodOrFieldDecl: Type Identifier InterfaceMethodOrFieldRest
                          ;

InterfaceMethodOrFieldRest: ConstantDeclaratorsRest SEMIC
                          | InterfaceMethodDeclaratorRest
                          ;

ConstantDeclaratorsRest: ConstantDeclarator
                       | ConstantDeclaratorRest COMMA ConstantDeclarator
                       ;

ConstantDeclaratorRest: Dims_opt ASGN VariableInitializer
                      ;

ConstantDeclarator: Identifier ConstantDeclaratorRest
                  ;

InterfaceMethodDeclaratorRest: FormalParameters Dims_opt Throws_opt SEMIC
                             ;

VoidInterfaceMethodDeclaratorRest: FormalParameters Throws_opt SEMIC

InterfaceGenericMethodDecl: TypeParameters Type Identifier InterfaceMethodDeclaratorRest
                          | TypeParameters VOID Identifier InterfaceMethodDeclaratorRest
                          ;


FormalParameters: LPAR FormalParameterDecls_opt RPAR
                ;

FormalParameterDecls_opt: %empty | FormalParameterDecls ;

FormalParameterDecls: Modifiers_opt Type FormalParameterDeclsRest
                    ;

FormalParameterDeclsRest: VariableDeclaratorId
                        | VariableDeclaratorId COMMA FormalParameterDecls
                        | DOTS VariableDeclaratorId
                        ;



VariableDeclaratorId: Identifier Dims_opt
                    ;



VariableDeclarators: VariableDeclarator
                   | VariableDeclarators COMMA VariableDeclarator
                   ;

VariableDeclarator: Identifier VariableDeclaratorRest
                  ;

VariableDeclaratorRest: Dims_opt VariableInitializerAssignment_opt
                      ;

VariableInitializerAssignment_opt: %empty | VariableInitializerAssignment ;

VariableInitializerAssignment: ASGN VariableInitializer
                             ;
VariableInitializers_opt: %empty | VariableInitializers ;

VariableInitializers: VariableInitializer
                    | VariableInitializers COMMA VariableInitializer
                    ;

VariableInitializer: ArrayInitializer
                   | Expression
                   ;

ArrayInitializer: LCUR RCUR
                | LCUR COMMA RCUR
                | LCUR VariableInitializers RCUR
                | LCUR VariableInitializers COMMA RCUR
                ;


Block: LCUR BlockStatements_opt RCUR
     ;

BlockStatements_opt: %empty | BlockStatements ;

BlockStatements: BlockStatement
               | BlockStatements BlockStatement
               ;

BlockStatement: LocalVariableDeclarationStatement
              | ClassOrInterfaceDeclaration
              | Statement
              ;

Label: Identifier COLON
     ;

LocalVariableDeclarationStatement: Type VariableDeclarators SEMIC
                                 | Modifiers Type VariableDeclarators SEMIC
                                 ;

Statement: Block
         | SEMIC
         | Label Statement
         | StatementExpression SEMIC
         | IF ParExpression Statement ELSE Statement
         | ASSERT Expression SEMIC
         | ASSERT Expression COLON Expression SEMIC
         | SWITCH ParExpression LCUR SwitchBlockStatementGroups RCUR
         | WHILE ParExpression Statement
         | DO Statement WHILE ParExpression SEMIC
         | FOR LPAR ForControl RPAR Statement
         | BREAK Identifier_opt SEMIC
         | CONTINUE Identifier_opt SEMIC
         | RETURN Expression_opt SEMIC
         | THROW Expression SEMIC
         | SYNCHRONIZED ParExpression Block
         | TRY Block Catches
         | TRY Block Catches_opt Finally
         | TRY ResourceSpecification Block Catches_opt Finally_opt
         ;

StatementExpression: Expression
                   ;

Catches_opt: %empty | Catches ;

Catches: CatchClause
       | Catches CatchClause
       ;

CatchClause: CATCH LPAR Modifiers_opt CatchType Identifier RPAR Block
           ;

CatchType: Name
         | CatchType VERT Name
         ;

Finally_opt: %empty | Finally ;

Finally: FINALLY Block
       ;

ResourceSpecification: LPAR Resources RPAR
                     | LPAR Resources SEMIC RPAR
                     ;

Resources: Resource
         | Resources SEMIC Resource
         ;

Resource: Modifiers_opt ReferenceType VariableDeclaratorId ASGN Expression
        ;


SwitchBlockStatementGroups: SwitchBlockStatementGroup
                          | SwitchBlockStatementGroups SwitchBlockStatementGroup
                          ;

SwitchBlockStatementGroup: SwitchLabels BlockStatements
                         ;

SwitchLabels: SwitchLabel
            | SwitchLabels SwitchLabel
            ;

SwitchLabel: CASE Expression COLON
           | DEFAULT COLON
           ;

EnumConstantName: Identifier
                ;



ForControl: ForInit SEMIC Expression_opt SEMIC ForUpdate_opt
          /* | ForVarControl */
          ;

ForVarControl: Modifiers_opt Type VariableDeclaratorId COLON Expression
             ;

ForVariableDeclaratorsRest: ForVariableDeclaratorsRestHead_opt ForVariableDeclaratorsRestTail_opt
                          ;

ForVariableDeclaratorsRestHead_opt: %empty | ForVariableDeclaratorsRestHead ;

ForVariableDeclaratorsRestHead: ASGN VariableInitializer
                              ;

ForVariableDeclaratorsRestTail_opt: %empty | ForVariableDeclaratorsRestTail ;

ForVariableDeclaratorsRestTail: COMMA VariableDeclarator
                              | ForVariableDeclaratorsRestTail COMMA VariableDeclarator
                              ;

ForInit: StatementExpressions
       ;

ForUpdate_opt: %empty | ForUpdate ;

ForUpdate: StatementExpressions
         ;

StatementExpressions: StatementExpression
                    | StatementExpressions COMMA StatementExpression
                    ;

Expressions_opt: %empty | Expressions ;

Expressions: Expression
           | Expressions COMMA Expression
           ;

Expression_opt: %empty | Expression ;

Expression: AssignmentExpression
          ;

AssignmentExpression: ConditionalExpression
                    | ConditionalExpression AssignmentOperator AssignmentExpression
                    ;

AssignmentOperator: ASGN
                  | COMP
                  ;

ConditionalExpression: ConditionalOrExpression
                     | ConditionalOrExpression INTRG Expression COLON ConditionalExpression
                     ;

ConditionalOrExpression: ConditionalAndExpression
                       | ConditionalOrExpression DVERT ConditionalAndExpression
                       ;

ConditionalAndExpression: InclusiveOrExpression
                        | ConditionalAndExpression DAMP InclusiveOrExpression
                        ;

InclusiveOrExpression: ExclusiveOrExpression
                     | InclusiveOrExpression VERT ExclusiveOrExpression
                     ;

ExclusiveOrExpression: AndExpression
                     | ExclusiveOrExpression HAT AndExpression
                     ;

AndExpression: EqualityExpression
             | AndExpression AMP EqualityExpression
             ;

EqualityExpression: RelationalExpression
                  | EqualityExpression EQOP RelationalExpression
                  | EqualityExpression INSTANCEOF ReferenceType
                  ;

RelationalExpression: ShiftExpression
                    | RelationalExpression LT ShiftExpression
                    | RelationalExpression GT ShiftExpression
                    | RelationalExpression RELOP ShiftExpression
                    ;

ShiftExpression: AdditiveExpression
               | ShiftExpression SHIFT AdditiveExpression
               ;


AdditiveExpression: MultiplicativeExpression
                  | AdditiveExpression ADDOP MultiplicativeExpression
                  ;

MultiplicativeExpression: UnaryExpression
                        | MultiplicativeExpression STAR UnaryExpression
                        | MultiplicativeExpression DIVOP UnaryExpression
                        ;

CastExpression: LPAR PrimitiveType RPAR UnaryExpression
              | LPAR ReferenceType RPAR UnaryExpressionNotPlusMinus
              ;

UnaryExpressionNotPlusMinus: PostfixExpression
                           | TILDE UnaryExpression
                           | EMPH UnaryExpression
                           /* | CastExpression TODO: resolve the conflict with ParExpression */
                           ;

PreIncDecExpression: INCDEC UnaryExpression
                   ;

UnaryExpression: PreIncDecExpression
               | ADDOP UnaryExpression
               | UnaryExpressionNotPlusMinus
               ;


PostIncDecExpression: PostfixExpression INCDEC
                    ;

PostfixExpression: Primary
                 | PostIncDecExpression
                 ;

Primary: Name
       | PrimaryNoName
       ;

PrimaryNoName: Literal
             | ParExpression
             | THIS
             | THIS Arguments
             | SUPER SuperSuffix
             | NEW Creator
             | NonWildcardTypeArguments ExplicitGenericInvocationSuffix
             | NonWildcardTypeArguments THIS Arguments
             | PrimitiveType Dims_opt DOT CLASS
             | VOID DOT CLASS
             | Name LBRA Expression RBRA
             | Name Arguments
             | Name DOT CLASS
             | Name DOT ExplicitGenericInvocation
             | Name DOT THIS
             | Name DOT SUPER Arguments
             | Name DOT NEW NonWildcardTypeArguments_opt InnerCreator
             | Name LBRA Dims DOT CLASS RBRA
             ;

Literal: INT_LIT
       | FLOAT_LIT
       | CHAR_LIT
       | STR_LIT
       | TRUE
       | FALSE
       | NULL
       ;

ParExpression: LPAR Expression RPAR
             ;

Arguments_opt: %empty | Arguments ;

Arguments: LPAR Expressions_opt RPAR
         ;

SuperSuffix: Arguments
           | DOT Identifier
           | DOT Identifier Arguments
           ;

ExplicitGenericInvocationSuffix: SUPER SuperSuffix
                               | Identifier Arguments
                               ;


Creator: NonWildcardTypeArguments CreatedName ClassCreatorRest
       | CreatedName ClassCreatorRest
       | CreatedName ArrayCreatorRest
       ;

CreatedName: Identifier CreatedNameTail_opt
           | Identifier TypeArgumentsOrDiamond CreatedNameTail_opt
           ;

CreatedNameTail_opt: %empty | CreatedNameTail ;

CreatedNameTail: DOT Identifier
               | DOT Identifier TypeArgumentsOrDiamond
               | CreatedNameTail DOT Identifier
               | CreatedNameTail DOT Identifier TypeArgumentsOrDiamond
               ;

ClassCreatorRest: Arguments
                | Arguments ClassBody
                ;

ArrayCreatorRest: LBRA RBRA ArrayInitializer
                | LBRA RBRA Dims ArrayInitializer
                | LBRA Expression RBRA ArrayCreatorExpressions
                | LBRA Expression RBRA ArrayCreatorExpressions Dims
                ;

ArrayCreatorExpressions_opt: %empty | ArrayCreatorExpressions ;

ArrayCreatorExpressions: LBRA Expression RBRA
                       | ArrayCreatorExpressions LBRA Expression RBRA
                       ;


IdentifierSuffix_opt: %empty | IdentifierSuffix ;

IdentifierSuffix: LBRA Expression RBRA
                | Arguments
                | DOT CLASS
                | DOT ExplicitGenericInvocation
                | DOT THIS
                | DOT SUPER Arguments
                | DOT NEW NonWildcardTypeArguments_opt InnerCreator
                | LBRA Dims DOT CLASS RBRA
                ;

ExplicitGenericInvocation: NonWildcardTypeArguments ExplicitGenericInvocationSuffix
                         ;

InnerCreator: Identifier NonWildcardTypeArgumentsOrDiamond_opt ClassCreatorRest
            ;

SelectorsNonDotIdentifier: SelectorDotNonIdentifier
                         | SelectorsNonDotIdentifier Selector
                         ;

Selectors: Selector
         | Selectors Selector
         ;

Selector: LBRA Expression RBRA
        | DOT SelectorDotNonIdentifier
        | DOT SelectorDotIdentifier
        ;

SelectorDotNonIdentifier: ExplicitGenericInvocation
                        | THIS
                        | SUPER SuperSuffix
                        | NEW NonWildcardTypeArguments_opt InnerCreator
                        ;

SelectorDotIdentifier: Identifier
                     | Identifier Arguments
                     ;


EnumBody: LCUR EnumConstants COMMA EnumBodyDeclarations RCUR
        | LCUR EnumConstants COMMA RCUR
        | LCUR COMMA EnumBodyDeclarations RCUR
        | LCUR EnumConstants RCUR
        | LCUR RCUR
        ;

EnumConstants: EnumConstant
             | EnumConstants COMMA EnumConstant
             ;

EnumConstant: Annotations_opt Identifier Arguments_opt ClassBody_opt
            ;

EnumBodyDeclarations: SEMIC ClassBodyDeclarations
                    ;



AnnotationTypeBody: LCUR AnnotationTypeElementDeclarations_opt RCUR
                  ;

AnnotationTypeElementDeclarations_opt: %empty | AnnotationTypeElementDeclarations ;

AnnotationTypeElementDeclarations: AnnotationTypeElementDeclaration
                                 | AnnotationTypeElementDeclarations AnnotationTypeElementDeclaration
                                 ;

AnnotationTypeElementDeclaration: Modifiers_opt AnnotationTypeElementRest
                                ;

AnnotationTypeElementRest: Type Identifier AnnotationMethodOrConstantRest SEMIC
                         | ClassDeclaration
                         | InterfaceDeclaration
                         ;

AnnotationMethodOrConstantRest: AnnotationMethodRest
                              | ConstantDeclaratorsRest
                              ;

AnnotationMethodRest: LPAR RPAR Dim_opt Default_opt
                    ;

Default_opt: %empty | Default ;

Default: DEFAULT ElementValue
       ;

%%

namespace jawa {

    void parser::report_syntax_error(parser::context const &parser_ctx) const
    {
        // Max 3 expected tokens
        symbol_kind_type expected[3];
        int n = parser_ctx.expected_tokens(expected, 3);

        std::stringstream msg;
        msg << "nieoczekiwany znak: " << symbol_name(parser_ctx.token());

        if (n) {
            msg << " oczekiwany znak:";
            for (int i = 0; i < n; ++i) {
                msg << ' ';
                msg << symbol_name(expected[i]);
                if (i + 1 < n)
                    msg << " albo";
            }
        }

        ctx->message(errors::SYNTAX, parser_ctx.location(), msg.str());
    }

    void parser::error(const location_type& loc, const std::string& msg)
    {
         ctx->message(errors::SYNTAX, loc, msg);
    }

}

