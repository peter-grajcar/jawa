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
%token<Name>                IDENTIFIER      "identyfikator"
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
                 | ClassOrInterfaceType DOT IDENTIFIER
                 ;

TypeVariable: IDENTIFIER
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

PackageName: IDENTIFIER
           | PackageName DOT IDENTIFIER
           ;

TypeName: IDENTIFIER
        | PackageOrTypeName DOT IDENTIFIER
        ;

ExpressionName: IDENTIFIER
              | AmbiguousName DOT IDENTIFIER
              ;

MethodName: IDENTIFIER
          | AmbiguousName DOT IDENTIFIER
          ;

PackageOrTypeName: IDENTIFIER
                 | PackageOrTypeName DOT IDENTIFIER
                 ;

AmbiguousName: IDENTIFIER
             | AmbiguousName DOT IDENTIFIER
             ;


CompilationUnit: %empty
               | PackageDeclaration ImportDeclarations TypeDeclarations
               | PackageDeclaration ImportDeclarations
               | PackageDeclaration TypeDeclarations
               | ImportDeclarations TypeDeclarations
               | PackageDeclaration
               | ImportDeclarations
               | TypeDeclarations
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

SingleStaticImportDeclaration: IMPORT STATIC TypeName DOT IDENTIFIER SEMIC
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

NormalClassDeclaration: ClassModifiers_opt CLASS IDENTIFIER TypeParameters Super Interfaces ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER TypeParameters Super ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER TypeParameters Interfaces ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER Super Interfaces ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER TypeParameters ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER Super ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER Interfaces ClassBody
                      | ClassModifiers_opt CLASS IDENTIFIER ClassBody
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
             | STRICTFP
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

VariableDeclaratorId: IDENTIFIER
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

MethodHeader: MethodModifiers TypeParameters Result MethodDeclarator Throws_opt
            | MethodModifiers Result MethodDeclarator Throws_opt
            | TypeParameters Result MethodDeclarator Throws_opt
            | Result MethodDeclarator Throws_opt
            ;

MethodDeclarator: IDENTIFIER LPAR FormalParameterList_opt RPAR
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
              | STRICTFP
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

ConstructorDeclaration: ConstructorModifiers_opt ConstructorDeclarator Throws_opt ConstructorBody

ConstructorDeclarator: TypeParameters_opt SimpleTypeName LPAR FormalParameterList_opt RPAR
                     ;

/* TODO: check if the name corresponds to the class name */
SimpleTypeName: IDENTIFIER
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

ConstructorBody: LCUR ExplicitConstructorInvocation BlockStatements RCUR
               | LCUR ExplicitConstructorInvocation RCUR
               | LCUR BlockStatements RCUR
               | LCUR RCUR
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

EnumDeclaration: ClassModifiers_opt ENUM IDENTIFIER Interfaces_opt EnumBody
               ;

COMMA_opt: %empty
         | COMMA
         ;

EnumBody: LCUR EnumConstants COMMA EnumBodyDeclarations RCUR
        | LCUR EnumConstants COMMA RCUR
        | LCUR EnumConstants EnumBodyDeclarations RCUR
        | LCUR COMMA EnumBodyDeclarations RCUR
        | LCUR EnumConstants RCUR
        | LCUR COMMA RCUR
        | LCUR EnumBodyDeclarations RCUR
        | LCUR RCUR
        ;

EnumConstants_opt: %empty
                 | EnumConstants
                 ;

EnumConstants: EnumConstant
             | EnumConstants COMMA EnumConstant
             ;

EnumConstant: Annotations_opt IDENTIFIER Arguments ClassBody
            | Annotations_opt IDENTIFIER Arguments
            | Annotations_opt IDENTIFIER ClassBody
            | Annotations_opt IDENTIFIER
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

InterfaceDeclaration: NormalInterfaceDeclaration
                    | AnnotationTypeDeclaration
                    ;

NormalInterfaceDeclaration: InterfaceModifiers_opt INTERFACE IDENTIFIER
                          | TypeParameters ExtendsInterfaces InterfaceBody
                          | TypeParameters InterfaceBody
                          | ExtendsInterfaces InterfaceBody
                          | InterfaceBody
                          ;

InterfaceModifiers_opt: %empty
                      | InterfaceModifiers
                      ;

InterfaceModifiers: InterfaceModifier
                  | InterfaceModifiers InterfaceModifier
                  ;

InterfaceModifier: Annotation
                 | PUBLIC
                 | PROTECTED
                 | PRIVATE
                 | ABSTRACT
                 | STATIC
                 | STRICTFP
                 ;

ExtendsInterfaces_opt: %empty
                     | ExtendsInterfaces
                     ;

ExtendsInterfaces: EXTENDS InterfaceTypeList
                 ;

InterfaceTypeList: InterfaceType
                 | InterfaceTypeList COMMA InterfaceType
                 ;

InterfaceType: TypeDeclSpecifier TypeArguments_opt
             ;

InterfaceBody: LCUR InterfaceMemberDeclarations_opt RCUR

InterfaceMemberDeclarations_opt: %empty
                               | InterfaceMemberDeclarations
                               ;

InterfaceMemberDeclarations: InterfaceMemberDeclaration
                           | InterfaceMemberDeclarations InterfaceMemberDeclaration
                           ;

InterfaceMemberDeclaration: ConstantDeclaration
                          | AbstractMethodDeclaration
                          | ClassDeclaration
                          | InterfaceDeclaration
                          | SEMIC
                          ;

ConstantDeclaration: ConstantModifiers_opt Type VariableDeclarators SEMIC
                   ;

ConstantModifiers_opt: %empty
                     | ConstantModifiers
                     ;

ConstantModifiers: ConstantModifier
                 | ConstantModifier ConstantModifiers
                 ;

ConstantModifier: Annotation
                | PUBLIC
                | STATIC
                | FINAL
                ;

AbstractMethodDeclaration: AbstractMethodModifiers TypeParameters Result MethodDeclarator Throws_opt SEMIC
                         | AbstractMethodModifiers Result MethodDeclarator Throws_opt SEMIC
                         | TypeParameters Result MethodDeclarator Throws_opt SEMIC
                         | Result MethodDeclarator Throws_opt SEMIC
                         ;

AbstractMethodModifiers_opt: %empty
                           | AbstractMethodModifiers
                           ;

AbstractMethodModifiers: AbstractMethodModifier
                       | AbstractMethodModifiers AbstractMethodModifier
                       ;

AbstractMethodModifier: Annotation
                      | PUBLIC
                      | ABSTRACT
                      ;

AnnotationTypeDeclaration: InterfaceModifiers_opt AT INTERFACE IDENTIFIER AnnotationTypeBody
                         ;

AnnotationTypeBody: LCUR AnnotationTypeElementDeclarations_opt RCUR
                  ;

AnnotationTypeElementDeclarations_opt: %empty
                                     | AnnotationTypeElementDeclarations
                                     ;

AnnotationTypeElementDeclarations: AnnotationTypeElementDeclaration
                                 | AnnotationTypeElementDeclarations AnnotationTypeElementDeclaration
                                 ;

AnnotationTypeElementDeclaration: AbstractMethodModifiers_opt Type IDENTIFIER LPAR RPAR Dims DefaultValue SEMIC
                                | AbstractMethodModifiers_opt Type IDENTIFIER LPAR RPAR Dims SEMIC
                                | AbstractMethodModifiers_opt Type IDENTIFIER LPAR RPAR DefaultValue SEMIC
                                | AbstractMethodModifiers_opt Type IDENTIFIER LPAR RPAR SEMIC
                                | ConstantDeclaration
                                | ClassDeclaration
                                | InterfaceDeclaration
                                | EnumDeclaration
                                | AnnotationTypeDeclaration
                                ;

DefaultValue_opt: %empty
                | DefaultValue
                ;

DefaultValue: DEFAULT ElementValue
            ;

Annotations_opt: %empty
               | Annotations
               ;

Annotations: Annotation
           | Annotations Annotation
           ;

Annotation: NormalAnnotation
          | MarkerAnnotation
          | SingleElementAnnotation
          ;

NormalAnnotation: AT TypeName LPAR ElementValuePairs_opt RPAR
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

ElementValues_opt: %empty
             | ElementValues
             ;

ElementValues: ElementValue
             | ElementValues COMMA ElementValue
             ;

MarkerAnnotation: AT IDENTIFIER
                ;

SingleElementAnnotation: AT IDENTIFIER LPAR ElementValue RPAR
                       ;

ArrayInitializer: LCUR VariableInitializers COMMA RCUR
                | LCUR VariableInitializers RCUR
                | LCUR COMMA RCUR
                | LCUR RCUR
                ;

VariableInitializers_opt: %empty
                        | VariableInitializers
                        ;

VariableInitializers: VariableInitializer
                    | VariableInitializers COMMA VariableInitializer
                    ;

VariableInitializer: Expression
                   | ArrayInitializer
                   ;

Block: LCUR BlockStatements_opt RCUR
     ;

BlockStatements_opt: %empty
                   | BlockStatements
                   ;

BlockStatements: BlockStatement
               | BlockStatements BlockStatement
               ;

BlockStatement: LocalVariableDeclarationStatement
              | ClassDeclaration
              | Statement
              ;

LocalVariableDeclarationStatement: LocalVariableDeclaration SEMIC
                                 ;

LocalVariableDeclaration: VariableModifiers_opt Type VariableDeclarators
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

VariableDeclarators: VariableDeclarator
                   | VariableDeclarators COMMA VariableDeclarator
                   ;

VariableDeclarator: VariableDeclaratorId
                  | VariableDeclaratorId ASGN VariableInitializer
                  ;

VariableDeclaratorId: IDENTIFIER
                    | VariableDeclaratorId LBRA RBRA
                    ;

VariableInitializer: Expression
                   | ArrayInitializer
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
                   | ClassInstanceCreationExpression
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

SwitchBlockStatementGroups_opt: %empty
                              | SwitchBlockStatementGroups
                              ;

SwitchBlockStatementGroups: SwitchBlockStatementGroup
                          | SwitchBlockStatementGroups SwitchBlockStatementGroup
                          ;

SwitchBlockStatementGroup: SwitchLabels BlockStatements
                         ;

SwitchLabels_opt: %empty
                | SwitchLabels
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
            | EnhancedForStatement
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

EnhancedForStatement: FOR LPAR FormalParameter COLON Expression RPAR Statement
                    ;

FormalParameter: VariableModifiers_opt Type VariableDeclaratorId
               ;

VariableDeclaratorId: IDENTIFIER
                    | VariableDeclaratorId LBRA RBRA
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

CatchFormalParameter: VariableModifiers_opt CatchType VariableDeclaratorId
                    ;

CatchType: ClassType
         | ClassType VERT CatchType
         ;

Finally_opt: %empty
           | Finally
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
         | Resource SEMIC Resources
         ;

Resource: VariableModifiers_opt Type VariableDeclaratorId ASGN Expression
        ;

/* Expressions */

Primary: PrimaryNoNewArray
       | ArrayCreationExpression
       ;

PrimaryNoNewArray: Literal
                 | Type DOT CLASS
                 | VOID DOT CLASS
                 | THIS
                 | ClassName DOT THIS
                 | LPAR Expression RPAR
                 | ClassInstanceCreationExpression
                 | FieldAccess
                 | MethodInvocation
                 | ArrayAccess
                 ;

ClassName: IDENTIFIER
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

ClassInstanceCreationExpression: NEW TypeArguments_opt TypeDeclSpecifier TypeArgumentsOrDiamond_opt LPAR ArgumentList_opt RPAR ClassBody_opt
                               | Primary DOT NEW TypeArguments_opt IDENTIFIER TypeArgumentsOrDiamond_opt LPAR ArgumentList_opt RPAR ClassBody_opt

TypeArgumentsOrDiamond_opt: %empty
                          | TypeArgumentsOrDiamond
                          ;

TypeArgumentsOrDiamond: TypeArguments
                      | LT GT
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
           | ClassName DOT SUPER DOT IDENTIFIER
           ;

MethodInvocation: MethodName LPAR ArgumentList_opt RPAR
                | Primary DOT NonWildTypeArguments_opt IDENTIFIER LPAR ArgumentList_opt RPAR
                | SUPER DOT NonWildTypeArguments_opt IDENTIFIER LPAR ArgumentList_opt RPAR
                | ClassName DOT SUPER DOT NonWildTypeArguments_opt IDENTIFIER LPAR ArgumentList_opt RPAR
                | TypeName DOT NonWildTypeArguments IDENTIFIER LPAR ArgumentList_opt RPAR

ArrayAccess: ExpressionName LBRA Expression RBRA
           | PrimaryNoNewArray LBRA Expression RBRA
           ;

PostfixExpression: Primary
                 | ExpressionName
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

LeftHandSide: ExpressionName
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

