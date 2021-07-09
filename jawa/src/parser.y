/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Peter Grajcar
 */
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
%token<Gender>              PRIVATE         "prywatny/prywatna"
%token<Gender>              PROTECTED       "chroniony/chroniona"
%token<Gender>              PUBLIC          "publiczny/publiczna"
%token<Gender>              STATIC          "statyczny/statyczna"
%token<Gender>              FINAL           "końcowy/końcowa"
%token<Gender>              NATIVE          "ojczysty/ojczysta"
%token<Gender>              VOLATILE        "zmeinny/zmeinna"
%token<Gender>              SYNCHRONIZED    "zsynchronizowany/zsynchronizowana"
%token<Gender>              ABSTRACT        "abstrakcyjny/abstrakcyjna"
%token<Gender>              TRANSIENT       "przejściowy/przejściowa"
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
%token                      SHL             "<<"
%token                      SHR             ">>"
%token                      USHR            ">>>"
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


%type<Name>                 Identifier Name
%type<NameList>             NameList

%%

/* Identifiers */

Identifier: IDF             { $$ = $1; }
          ;

Name: Identifier            { $$ = $1;  }
    | Name DOT Identifier   { $$ = $1 + "/" + $3; }
    ;

NameList: Name                  { $$.push_back($1); }
        | NameList COMMA Name   { $$ = $1; $$.push_back($3); }
        ;

/* Declarations */

CompilationUnit: PackageDeclaration_opt ImportDeclarations_opt TypeDeclarations_opt
               ;

PackageDeclaration_opt: %empty
                      | PackageDeclaration
                      ;

PackageDeclaration: PACKAGE Name SEMIC
                  ;

ImportDeclarations_opt: %empty
                      | ImportDeclarations
                      ;

ImportDeclarations: ImportDeclaration
                  | ImportDeclarations ImportDeclaration
                  ;

ImportDeclaration: IMPORT Name SEMIC
                 ;

TypeDeclarations_opt: %empty
                    | TypeDeclarations
                    ;

TypeDeclarations: TypeDeclaration
                | TypeDeclarations TypeDeclaration
                ;

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

NormalInterfaceDeclaration: INTERFACE Identifier TypeParameters_opt InterfaceExtends_opt InterfaceBody
                          ;

AnnotationTypeDeclaration: AT_INTERFACE Identifier AnnotationTypeBody
                         ;

ClassExtends_opt: %empty
                | ClassExtends
                ;

ClassExtends: EXTENDS Type
            ;

InterfaceExtends_opt: %empty
                    | InterfaceExtends
                    ;

InterfaceExtends: EXTENDS TypeList
                ;

Implements_opt: %empty
              | Implements
              ;

Implements: IMPLEMENTS TypeList
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
             | ArrayType
             ;

ReferenceTypeNoName: ClassOrInterfaceTypeNoName
                   | ArrayType
                   ;

ClassOrInterfaceTypeNoName: Name TypeArguments
                          | TypeDeclSpecifier
                          | TypeDeclSpecifier TypeArguments
                          ;

ClassOrInterfaceType: Name
                    | Name TypeArguments
                    | TypeDeclSpecifier
                    | TypeDeclSpecifier TypeArguments
                    ;

TypeDeclSpecifier: TypeDeclSpecifierHead Name
                 ;

TypeDeclSpecifierHead: Name TypeArguments DOT
                     | TypeDeclSpecifierHead DOT Name TypeArguments DOT
                     ;

ArrayType: PrimitiveType Dims
         | Name Dims
         | TypeDeclSpecifier Dims
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

TypeArguments: LT TypeArgumentList GT
             | LT Name LT TypeArgumentList SHR
             | LT TypeDeclSpecifier LT TypeArgumentList SHR
             | LT TypeArgumentListHeads Name LT TypeArgumentList SHR
             | LT TypeArgumentListHeads TypeDeclSpecifier LT TypeArgumentList SHR
             /* TODO: add USHR */
             ;

TypeArgumentListHeads: TypeArgumentListHead
                     | TypeArgumentListHeads TypeArgumentListHead
                     ;

TypeArgumentListHead: TypeArgument COMMA
                    ;

TypeArgumentList: TypeArgument
                | TypeArgumentListHeads TypeArgument
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


TypeArgumentsOrDiamond: LT GT
                      | TypeArguments
                      ;

NonWildcardTypeArgumentsOrDiamond_opt: %empty | NonWildcardTypeArgumentsOrDiamond ;

NonWildcardTypeArgumentsOrDiamond: LT GT
                                 | NonWildcardTypeArguments
                                 ;


TypeParameters_opt: %empty
                  | TypeParameters
                  ;

TypeParameters: LT TypeParameterList GT
              ;

TypeParameterList: TypeParameter
                 | TypeParameterList COMMA TypeParameter
                 ;

TypeParameter: Identifier TypeParameterBound_opt
             ;

TypeParameterBound_opt: %empty
                      | TypeParameterBound
                      ;

TypeParameterBound: EXTENDS Bound
                  ;

Bound: ReferenceType
     | Bound AMP ReferenceType
     ;

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
        | STATIC
        | ABSTRACT
        | FINAL
        | NATIVE
        | SYNCHRONIZED
        | TRANSIENT
        | VOLATILE
        | STRICTFP
        ;

Annotation: AT Name
          | AT Name LPAR AnnotationElement_opt RPAR
          ;

AnnotationElement_opt: %empty
                     | AnnotationElement
                     ;

AnnotationElement: ElementValuePairs
                 | ElementValue
                 ;

ElementValuePairs: ElementValuePair
                 | ElementValuePairs COMMA ElementValuePair
                 ;

ElementValuePair: Identifier ASGN ElementValue
                ;

ElementValue: Annotation
            | ConditionalExpressionNoName
            | Name
            | ElementValueArrayInitializer
            ;

ElementValueArrayInitializer: LCUR RCUR
                            | LCUR COMMA RCUR
                            | LCUR ElementValues RCUR
                            | LCUR ElementValues COMMA RCUR
                            ;

ElementValues: ElementValue
             | ElementValues COMMA ElementValue
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

FieldDeclaratorsRest: VariableDeclaratorRest
                    | VariableDeclaratorRest FieldDeclaratorsRestTail
                    ;

FieldDeclaratorsRestTail: COMMA VariableDeclarator
                        | FieldDeclaratorsRestTail COMMA VariableDeclarator
                        ;

MethodDeclaratorRest: FormalParameters Dims_opt Throws_opt Block
                    | FormalParameters Dims_opt Throws_opt SEMIC
                    ;

Throws_opt: %empty
          | Throws
          ;

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

FormalParameterDecls_opt: %empty
                        | FormalParameterDecls
                        ;

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

VariableInitializerAssignment_opt: %empty
                                 | VariableInitializerAssignment
                                 ;

VariableInitializerAssignment: ASGN VariableInitializer
                             ;

VariableInitializers: VariableInitializer
                    | VariableInitializers COMMA VariableInitializer
                    ;

VariableInitializer: ArrayInitializer
                   | ExpressionNoName
                   | Name
                   ;

ArrayInitializer: LCUR RCUR
                | LCUR COMMA RCUR
                | LCUR VariableInitializers RCUR
                | LCUR VariableInitializers COMMA RCUR
                ;


Block: LCUR BlockStatements_opt RCUR
     ;

BlockStatements_opt: %empty
                   | BlockStatements
                   ;

BlockStatements: BlockStatement
               | BlockStatements BlockStatement
               ;

BlockStatement: LocalVariableDeclarationStatement SEMIC
              | ClassOrInterfaceDeclaration
              | Statement
              ;

LocalVariableDeclarationStatement: Type VariableDeclarators
                                 | Modifiers Type VariableDeclarators
                                 ;
/* Statements */

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

IfThenStatement: IF LPAR ExpressionNoName RPAR Statement
               | IF LPAR Name RPAR Statement
               ;

IfThenElseStatement: IF LPAR ExpressionNoName RPAR StatementNoShortIf ELSE Statement
                   | IF LPAR Name RPAR StatementNoShortIf ELSE Statement
                   ;

IfThenElseStatementNoShortIf: IF LPAR ExpressionNoName RPAR StatementNoShortIf ELSE StatementNoShortIf
                            | IF LPAR Name RPAR StatementNoShortIf ELSE StatementNoShortIf
                            ;

LabeledStatement: Identifier COLON Statement
                ;

LabeledStatementNoShortIf: Identifier COLON StatementNoShortIf
                         ;

ExpressionStatement: StatementExpression SEMIC
                   ;

StatementExpression: ExpressionNoName
                   | Name
                   ;

AssertStatement: ASSERT ExpressionNoName SEMIC
               | ASSERT Name SEMIC
               | ASSERT ExpressionNoName COLON ExpressionNoName SEMIC
               | ASSERT ExpressionNoName COLON Name SEMIC
               | ASSERT Name COLON ExpressionNoName SEMIC
               | ASSERT Name COLON Name SEMIC
               ;

SwitchStatement: SWITCH LPAR ExpressionNoName RPAR SwitchBlock
               | SWITCH LPAR Name RPAR SwitchBlock
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

SwitchLabel: CASE ConstantExpressionNoName COLON
           | CASE Name COLON
           | DEFAULT COLON
           ;

WhileStatement: WHILE LPAR ExpressionNoName RPAR Statement
              | WHILE LPAR Name RPAR Statement
              ;

WhileStatementNoShortIf: WHILE LPAR ExpressionNoName RPAR StatementNoShortIf
                       | WHILE LPAR Name RPAR StatementNoShortIf
                       ;

DoStatement: DO Statement WHILE LPAR ExpressionNoName RPAR SEMIC
           | DO Statement WHILE LPAR Name RPAR SEMIC
           ;

ForStatement: BasicForStatement
            ;

BasicForStatement: FOR LPAR ForInit_opt SEMIC ExpressionNoName_opt SEMIC ForUpdate_opt RPAR Statement
                 | FOR LPAR ForInit_opt SEMIC Name SEMIC ForUpdate_opt RPAR Statement
                 ;

ForStatementNoShortIf: FOR LPAR ForInit_opt SEMIC ExpressionNoName_opt SEMIC ForUpdate_opt RPAR StatementNoShortIf
                     | FOR LPAR ForInit_opt SEMIC Name SEMIC ForUpdate_opt RPAR StatementNoShortIf
                     ;

ForInit_opt: %empty
           | ForInit
           ;

ForInit: LocalVariableDeclarationStatement
       /* | StatementExpression */
       ;

ForUpdate_opt: %empty
             | ForUpdate
             ;

ForUpdate: StatementExpressionList
         ;

StatementExpressionList: StatementExpression
                       | StatementExpressionList COMMA StatementExpression
                       ;

BreakStatement: BREAK SEMIC
              | BREAK Identifier SEMIC
              ;

ContinueStatement: CONTINUE SEMIC
                 | CONTINUE Identifier SEMIC
                 ;

ReturnStatement: RETURN ExpressionNoName SEMIC
               | RETURN Name SEMIC
               | RETURN SEMIC
               ;

ThrowStatement: THROW ExpressionNoName SEMIC
              | THROW Name SEMIC
              ;

SynchronizedStatement: SYNCHRONIZED LPAR ExpressionNoName RPAR Block
                     | SYNCHRONIZED LPAR Name RPAR Block
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

CatchType: ClassOrInterfaceType
         | ClassOrInterfaceType VERT CatchType
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

Resource: Modifiers_opt Type VariableDeclaratorId ASGN ExpressionNoName
        | Modifiers_opt Type VariableDeclaratorId ASGN Name
        ;


/* Expressions */

Expressions_opt: %empty | Expressions ;

Expressions: ExpressionNoName
           | Name
           | Expressions COMMA ExpressionNoName
           | Expressions COMMA Name
           ;

ExpressionNoName_opt: %empty
                    | ExpressionNoName
                    ;

ExpressionNoName: AssignmentExpressionNoName
          ;

AssignmentExpressionNoName: ConditionalExpressionNoName
                    | ConditionalExpressionNoName AssignmentOperator AssignmentExpressionNoName
                    | ConditionalExpressionNoName AssignmentOperator Name
                    | Name AssignmentOperator AssignmentExpressionNoName
                    | Name AssignmentOperator Name
                    ;

AssignmentOperator: ASGN
                  | COMP
                  ;

ConditionalExpressionNoName: ConditionalOrExpressionNoName
                           | ConditionalOrExpressionNoName INTRG ExpressionNoName COLON ConditionalExpressionNoName
                           | ConditionalOrExpressionNoName INTRG ExpressionNoName COLON Name
                           | ConditionalOrExpressionNoName INTRG Name COLON ConditionalExpressionNoName
                           | Name INTRG ExpressionNoName COLON ConditionalExpressionNoName
                           | ConditionalOrExpressionNoName INTRG Name COLON Name
                           | Name INTRG ExpressionNoName COLON Name
                           | Name INTRG Name COLON ConditionalExpressionNoName
                           | Name INTRG Name COLON Name
                           ;

ConditionalOrExpressionNoName: ConditionalAndExpressionNoName
                             | ConditionalOrExpressionNoName DVERT ConditionalAndExpressionNoName
                             | ConditionalOrExpressionNoName DVERT Name
                             | Name DVERT ConditionalAndExpressionNoName
                             | Name DVERT Name
                             ;

ConditionalAndExpressionNoName: InclusiveOrExpressionNoName
                              | ConditionalAndExpressionNoName DAMP InclusiveOrExpressionNoName
                              | ConditionalAndExpressionNoName DAMP Name
                              | Name DAMP InclusiveOrExpressionNoName
                              | Name DAMP Name
                              ;

InclusiveOrExpressionNoName: ExclusiveOrExpressionNoName
                           | InclusiveOrExpressionNoName VERT ExclusiveOrExpressionNoName
                           | InclusiveOrExpressionNoName VERT Name
                           | Name VERT ExclusiveOrExpressionNoName
                           | Name VERT Name
                           ;

ExclusiveOrExpressionNoName: AndExpressionNoName
                           | ExclusiveOrExpressionNoName HAT AndExpressionNoName
                           | ExclusiveOrExpressionNoName HAT Name
                           | Name HAT AndExpressionNoName
                           | Name HAT Name
                           ;

AndExpressionNoName: EqualityExpressionNoName
                   | AndExpressionNoName AMP EqualityExpressionNoName
                   | AndExpressionNoName AMP Name
                   | Name AMP EqualityExpressionNoName
                   | Name AMP Name
                   ;

EqualityExpressionNoName: RelationalExpressionNoName
                        | EqualityExpressionNoName EQOP RelationalExpressionNoName
                        | EqualityExpressionNoName EQOP Name
                        | Name EQOP RelationalExpressionNoName
                        | Name EQOP Name
                        | EqualityExpressionNoName INSTANCEOF ReferenceType
                        | Name INSTANCEOF ReferenceType
                        ;

RelationalExpressionNoName: ShiftExpressionNoName
                          | ShiftExpressionNoName LT ShiftExpressionNoName
                          | ShiftExpressionNoName LT Name
                          | Name LT ShiftExpressionNoName
                          | Name LT Name
                          | ShiftExpressionNoName GT ShiftExpressionNoName
                          | ShiftExpressionNoName GT Name
                          | Name GT ShiftExpressionNoName
                          | Name GT Name
                          | ShiftExpressionNoName RELOP ShiftExpressionNoName
                          | ShiftExpressionNoName RELOP Name
                          | Name RELOP ShiftExpressionNoName
                          | Name RELOP Name
                          ;

ShiftExpressionNoName: AdditiveExpressionNoName
                     | ShiftExpressionNoName SHL AdditiveExpressionNoName
                     | ShiftExpressionNoName SHL Name
                     | ShiftExpressionNoName SHR AdditiveExpressionNoName
                     | ShiftExpressionNoName SHR Name
                     | ShiftExpressionNoName USHR AdditiveExpressionNoName
                     | ShiftExpressionNoName USHR Name
                     | Name SHL AdditiveExpressionNoName
                     | Name SHL Name
                     | Name SHR AdditiveExpressionNoName
                     | Name SHR Name
                     | Name USHR AdditiveExpressionNoName
                     | Name USHR Name
                     ;


AdditiveExpressionNoName: MultiplicativeExpressionNoName
                        | AdditiveExpressionNoName ADDOP MultiplicativeExpressionNoName
                        | AdditiveExpressionNoName ADDOP Name
                        | Name ADDOP MultiplicativeExpressionNoName
                        | Name ADDOP Name
                        ;

MultiplicativeExpressionNoName: UnaryExpressionNoName
                              | MultiplicativeExpressionNoName STAR UnaryExpressionNoName
                              | MultiplicativeExpressionNoName STAR Name
                              | Name STAR UnaryExpressionNoName
                              | Name STAR Name
                              | MultiplicativeExpressionNoName DIVOP UnaryExpressionNoName
                              | MultiplicativeExpressionNoName DIVOP Name
                              | Name DIVOP UnaryExpressionNoName
                              | Name DIVOP Name
                              ;

CastExpressionNoName: LPAR PrimitiveType RPAR UnaryExpressionNoName
                    | LPAR PrimitiveType RPAR Name
                    | LPAR ReferenceTypeNoName RPAR UnaryExpressionNotPlusMinusNoName
                    | LPAR ReferenceTypeNoName RPAR Name
                    | LPAR Name RPAR UnaryExpressionNotPlusMinusNoName
                    | LPAR Name RPAR Name
                    ;

UnaryExpressionNotPlusMinusNoName: PostfixExpressionNoName
                                 | TILDE UnaryExpressionNoName
                                 | TILDE Name
                                 | EMPH UnaryExpressionNoName
                                 | EMPH Name
                                 | CastExpressionNoName
                                 ;

PreIncDecExpression: INCDEC UnaryExpressionNoName
                   | INCDEC Name
                   ;

UnaryExpressionNoName: PreIncDecExpression
                     | ADDOP UnaryExpressionNoName
                     | ADDOP Name
                     | UnaryExpressionNotPlusMinusNoName
                     ;


PostIncDecExpression: PostfixExpressionNoName INCDEC
                    | Name INCDEC
                    ;

PostfixExpressionNoName: PrimaryNoName
                       | PostIncDecExpression
                       ;

ConstantExpressionNoName: ExpressionNoName
                        ;

PrimaryNoName: Literal
             | LPAR ExpressionNoName RPAR
             | LPAR Name RPAR
             | THIS ThisSuffix
             | SUPER SuperSuffix
             | NEW Creator
             /* | NonWildcardTypeArguments ExplicitGenericInvocationSuffix */
             /* | NonWildcardTypeArguments THIS Arguments */
             | PrimitiveType Dims_opt DOT CLASS
             | VOID DOT CLASS
             | Name LBRA ExpressionNoName RBRA
             | Name LBRA Name RBRA
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

Arguments_opt: %empty | Arguments ;

Arguments: LPAR Expressions_opt RPAR
         ;

SuperSuffix: Arguments
           | DOT Identifier
           | DOT Identifier Arguments
           ;

ThisSuffix: Arguments
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
                | LBRA ExpressionNoName RBRA ArrayCreatorExpressions
                | LBRA ExpressionNoName RBRA ArrayCreatorExpressions Dims
                ;

ArrayCreatorExpressions: LBRA ExpressionNoName RBRA
                       | ArrayCreatorExpressions LBRA ExpressionNoName RBRA
                       ;

ExplicitGenericInvocation: NonWildcardTypeArguments ExplicitGenericInvocationSuffix
                         ;

InnerCreator: Identifier NonWildcardTypeArgumentsOrDiamond_opt ClassCreatorRest
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

EnumConstant: Modifiers_opt Identifier Arguments_opt ClassBody_opt
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

        Name unexpected_token = symbol_name(parser_ctx.token());

        std::stringstream expected_tokens;
        if (n) {
            for (int i = 0; i < n; ++i) {
                expected_tokens << symbol_name(expected[i]);
                if (i + 1 < n)
                    expected_tokens << ", ";
            }
        }

        ctx->message(errors::SYNTAX, parser_ctx.location(), unexpected_token, expected_tokens.str());
    }

    void parser::error(const location_type& loc, const std::string& msg) {}

}

