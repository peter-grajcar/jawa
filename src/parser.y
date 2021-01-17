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
%token                      AMP             "&"
%token                      DAMP            "&&"
%token                      VERT            "|"
%token                      DVERT           "||"
%token                      HAT             "^"
%token                      TILDE           "~"
%token                      ASGN            "="
%token                      COLON           ":"
%token                      INTRG           "?"
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
%token<Name>                IDF             "identyfikator"
%token<Name>                STR_LIT         "łancuch"
%token<int_t>               INT_LIT         "literał całkowity"
%token                      TRUE            "prawda"
%token                      FALSE           "nieprawda"

%%

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
             | TypeVariable
             | ArrayType
             ;

ClassOrInterfaceType: ClassType
                    | InterfaceType
                    ;

ClassType: TypeDeclSpecifier TypeArguments_opt
         ;

InterfaceType: TypeDeclSpecifier TypeArguments_opt
             ;

TypeDeclSpecifier: TypeName
                 | ClassOrInterfaceType DOT IDF
                 ;

TypeVariable: IDF
            ;

TypeParameter: TypeVariable TypeBound_opt

TypeBound_opt: %empty
             | TypeBound
             ;

TypeBound: EXTENDS TypeVariable
         | EXTENDS ClassOrInterfaceType AdditionalBoundList_opt
         ;

AdditionalBoundList_opt: %empty
                       | AdditionalBoundList
                       ;

AdditionalBoundList: AdditionalBound AdditionalBoundList
                   | AdditionalBound
                   ;

AdditionalBound: AMP InterfaceType
               ;

TypeArguments_opt: %empty
                 | TypeArguments
                 ;

TypeArguments: LT TypeArgumentList GT
             ;

TypeArgumentList: TypeArgument
                | TypeArgumentList COMMA TypeArgument
                ;

TypeArgument: ReferenceType
            | Wildcard
            ;

Wildcard: INTRG WildcardBounds_opt
        ;

WildcardBounds_opt: %empty
                  | WildcardBounds
                  ;

WildcardBounds: EXTENDS ReferenceType
              | SUPER ReferenceType
              ;

ArrayType: Type LBRA RBRA
         ;

PackageName: IDF
           | PackageName DOT IDF
           ;

TypeName: IDF
        | PackageOrTypeName DOT IDF
        ;

ExpressionName: IDF
              | AmbiguousName DOT IDF
              ;

MethodName: IDF
          | AmbiguousName DOT IDF
          ;

PackageOrTypeName: IDF
                 | PackageOrTypeName DOT IDF
                 ;

AmbiguousName: IDF
             | AmbiguousName DOT IDF
             ;


CompilationUnit: PackageDeclaration_opt ImportDeclarations_opt TypeDeclarations_opt
                  ;

ImportDeclarations_opt: %empty
                      | ImportDeclarations
                      ;

ImportDeclarations: ImportDeclaration
                  | ImportDeclarations ImportDeclaration
                  ;

TypeDeclarations_opt: %empty
                    | TypeDeclarations
                    ;

TypeDeclarations: TypeDeclaration
                | TypeDeclarations TypeDeclaration
                ;

PackageDeclaration_opt: %empty
                      | PackageDeclaration
                      ;

PackageDeclaration: Annotations_opt PACKAGE PackageName SEMIC
                  ;

ImportDeclaration: SingleTypeImportDeclaration
                 | TypeImportOnDemandDeclaration
                 | SingleStaticImportDeclaration
                 | StaticImportOnDemandDeclaration
                 ;

SingleTypeImportDeclaration: IMPORT TypeName SEMIC
                           ;

TypeImportOnDemandDeclaration: IMPORT PackageOrTypeName DOT STAR SEMIC
                             ;

SingleStaticImportDeclaration: IMPORT STATIC TypeName DOT IDF SEMIC
                             ;

StaticImportOnDemandDeclaration: IMPORT STATIC TypeName DOT STAR SEMIC
                               ;

TypeDeclaration: ClassDeclaration
               | InterfaceDeclaration
               | SEMIC
               ;

ClassDeclaration: NormalClassDeclaration
                | EnumDeclaration
                ;

NormalClassDeclaration: ClassModifiers_opt CLASS IDF TypeParameters_opt Super_opt Interfaces_opt ClassBody
                      ;

ClassModifiers_opt: %empty
                  | ClassModifiers
                  ;

ClassModifiers: ClassModifier
              | ClassModifiers ClassModifier
              ;

ClassModifier: Annotation
             | PUBLIC
             | PROTECTED
             | PRIVATE
             | ABSTRACT
             | STATIC
             | FINAL
             ;

TypeParameters_opt: %empty
                  | TypeParameters
                  ;

TypeParameters: LT TypeParameterList GT
              ;

TypeParameterList: TypeParameterList COMMA TypeParameter
                 | TypeParameter
                 ;

Super_opt: %empty
         | Super
         ;

Super: EXTENDS ClassType
     ;

ClassType: TypeDeclSpecifier TypeArguments_opt
         ;

Interfaces_opt: %empty
              | Interfaces
              ;

Interfaces: IMPLEMENTS InterfaceTypeList
          ;

InterfaceTypeList: InterfaceType
                 | InterfaceTypeList COMMA InterfaceType
                 ;

ClassBody_opt: %empty
             | ClassBody
             ;

ClassBody: LCUR ClassBodyDeclarations_opt RCUR
         ;

ClassBodyDeclarations_opt: %empty
                         | ClassBodyDeclarations
                         ;

ClassBodyDeclarations: ClassBodyDeclaration
                     | ClassBodyDeclarations ClassBodyDeclaration

ClassBodyDeclaration: ClassMemberDeclaration
                    | InstanceInitializer
                    | StaticInitializer
                    | ConstructorDeclaration
                    ;

ClassMemberDeclaration: FieldDeclaration
                      | MethodDeclaration
                      | ClassDeclaration
                      | InterfaceDeclaration
                      ;

FieldDeclaration: FieldModifiers_opt Type VariableDeclarators SEMIC
                ;

VariableDeclarators: VariableDeclarator
                   | VariableDeclarators COMMA VariableDeclarator
                   ;

VariableDeclarator: VariableDeclaratorId
                  | VariableDeclaratorId ASGN VariableInitializer
                  ;

VariableDeclaratorId: IDF
                    | VariableDeclaratorId LBRA RBRA
                    ;

VariableInitializer: Expression
                   | ArrayInitializer
                   ;

FieldModifiers_opt: %empty
                  | FieldModifiers
                  ;

FieldModifiers: FieldModifier
              | FieldModifiers FieldModifier

FieldModifier: Annotation
             | PUBLIC
             | PROTECTED
             | PRIVATE
             | STATIC
             | FINAL
             | TRANSIENT
             | VOLATILE
             ;

MethodDeclaration: MethodHeader MethodBody
                 ;

MethodHeader: MethodModifiers_opt TypeParameters_opt Result MethodDeclarator Throws_opt
            ;

MethodDeclarator: IDF LPAR FormalParameterList_opt RPAR
                ;

MethodDeclarator: MethodDeclarator LBRA RBRA
                ;

FormalParameterList_opt: %empty
                       | FormalParameterList
                       ;

FormalParameterList: LastFormalParameter
                   | FormalParameters COMMA LastFormalParameter
                   ;

FormalParameters: FormalParameter
                | FormalParameters COMMA FormalParameter
                ;

FormalParameter: VariableModifiers_opt Type VariableDeclaratorId
               ;

VariableModifiers_opt: %empty
                     | VariableModifiers
                     ;

VariableModifiers: VariableModifier
                 | VariableModifiers VariableModifier
                 ;

VariableModifier: Annotation
                | FINAL
                ;

LastFormalParameter: VariableModifiers_opt Type DOTS VariableDeclaratorId
                   | FormalParameter
                   ;

MethodModifiers_opt: %empty
                   | MethodModifiers
                   ;

MethodModifiers: MethodModifier
               | MethodModifiers MethodModifier
               ;

MethodModifier: Annotation
              | PUBLIC
              | PROTECTED
              | PRIVATE
              | ABSTRACT
              | STATIC
              | FINAL
              | SYNCHRONIZED
              | NATIVE
              ;

Result: Type
      | VOID
      ;

Throws_opt: %empty
          | Throws
          ;

Throws: THROWS ExceptionTypeList
      ;

ExceptionTypeList: ExceptionType
                 | ExceptionTypeList COMMA ExceptionType
                 ;

ExceptionType: TypeName
             | TypeVariable
             ;

MethodBody: Block
          ;

InstanceInitializer: Block
                   ;

StaticInitializer: STATIC Block
                 ;

/* TODO: */
Block: LCUR RCUR
     ;

ConstructorDeclaration: ConstructorModifiers_opt ConstructorDeclarator Throws_opt ConstructorBody

ConstructorDeclarator: TypeParameters_opt SimpleTypeName LPAR FormalParameterList_opt RPAR
                     ;

/* TODO: check if the name corresponds to the class name */
SimpleTypeName: IDF
              ;

ConstructorModifiers_opt: %empty
                        | ConstructorModifiers
                        ;

ConstructorModifiers: ConstructorModifier
                    | ConstructorModifiers ConstructorModifier
                    ;

ConstructorModifier: Annotation
                   | PUBLIC
                   | PROTECTED
                   | PRIVATE
                   ;

ConstructorBody: LCUR ExplicitConstructorInvocation_opt BlockStatements_opt RCUR
               ;

ExplicitConstructorInvocation_opt: %empty
                                 | ExplicitConstructorInvocation
                                 ;

ExplicitConstructorInvocation: NonWildTypeArguments_opt THIS LPAR ArgumentList_opt RPAR SEMIC
                             | NonWildTypeArguments_opt SUPER LPAR ArgumentList_opt RPAR SEMIC
                             | Primary DOT NonWildTypeArguments_opt SUPER LPAR ArgumentList_opt RPAR SEMIC
                             ;

NonWildTypeArguments_opt: %empty
                        | NonWildTypeArguments
                        ;

NonWildTypeArguments: LT ReferenceTypeList GT
                    ;

ReferenceTypeList: ReferenceType
                 | ReferenceTypeList COMMA ReferenceType
                 ;

EnumDeclaration: ClassModifiers_opt ENUM IDF Interfaces_opt EnumBody
               ;

COMMA_opt: %empty
         | COMMA
         ;

EnumBody: LCUR EnumConstants_opt COMMA_opt EnumBodyDeclarations_opt RCUR
        ;

EnumConstants_opt: %empty
                 | EnumConstants
                 ;

EnumConstants: EnumConstant
             | EnumConstants COMMA EnumConstant
             ;

EnumConstant: Annotations_opt IDF Arguments_opt ClassBody_opt
            ;

Arguments_opt: %empty
             | Arguments
             ;

Arguments: LPAR ArgumentList_opt RPAR
         ;

EnumBodyDeclarations_opt: %empty
                        | EnumBodyDeclarations
                        ;

EnumBodyDeclarations: SEMIC ClassBodyDeclarations_opt
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

