#ifndef __RULENUMBERS_H__
#define __RULENUMBERS_H__

// Rules

enum RuleNumbers {
    GOAL = 0,                       // 0 Goal CompilationUnit
    COMPILATION_UNIT_CLASS,         // 1 CompilationUnit PackageDeclaration ImportDeclarations* ClassDeclaration
    COMPILATION_UNIT_INTERFACE,     // 2 CompilationUnit PackageDeclaration ImportDeclarations* InterfaceDeclaration
    PACKAGE_NAME,                   // 3 PackageDeclaration package Name ;
    PACKAGE_EPSILON,                // 4 PackageDeclaration
    IMPORT_STAR_DECLS,              // 5 ImportDeclarations* ImportDeclarations
    IMPORT_STAR_EPSILON,            // 6 ImportDeclarations*
    IMPORTS_DECL,                   // 7 ImportDeclarations ImportDeclaration
    IMPORTS_LIST,                   // 8 ImportDeclarations ImportDeclarations ImportDeclaration
    SINGLE_TYPE_IMPORT,             // 9 ImportDeclaration SingleTypeImportDeclaration
    IMPORT_ON_DEMAND,               // 10 ImportDeclaration TypeImportOnDemandDeclaration
    IMPORT_NAME,                    // 11 SingleTypeImportDeclaration import Name ;
    IMPORT_NAME_STAR,               // 12 TypeImportOnDemandDeclaration import Name . * ;
    NAME_SIMPLE,                    // 13 Name SimpleName
    NAME_QUALIFIED,                 // 14 Name QualifiedName
    SIMPLE,                         // 15 SimpleName Identifier
    QUALIFIED,                      // 16 QualifiedName Name . Identifier
    IDENTIFIER,                     // 17 Identifier ID
    CLASS_DECL,                     // 18 ClassDeclaration Modifiers class Identifier Super Interfaces ClassBody
    MODIFIERS_MOD,                  // 19 Modifiers Modifier
    MODIFIERS_LIST,                 // 20 Modifiers Modifiers Modifier
    MOD_PUBLIC,                     // 21 Modifier public
    MOD_ABSTRACT,                   // 22 Modifier abstract
    MOD_FINAL,                      // 23 Modifier final
    EXTENDS_CLASS,                  // 24 Super extends ClassType
    EXTENDS_CLASS_EPSILON,          // 25 Super
    CLASS_TYPE,                     // 26 ClassType ClassOrInterfaceType
    CLASS_OR_INTERFACE_NAME,        // 27 ClassOrInterfaceType Name
    IMPLEMENTING,                   // 28 Interfaces implements InterfaceTypeList
    IMPLEMENT_EPSILON,              // 29 Interfaces
    INTERFACE_TYPE_LIST_END,        // 30 InterfaceTypeList InterfaceType
    INTERFACE_TYPE_LIST,            // 31 InterfaceTypeList InterfaceTypeList , InterfaceType
    INTERFACE_TYPE,                 // 32 InterfaceType ClassOrInterfaceType
    CLASS_BODY,                     // 33 ClassBody { ClassBodyDeclarations* }
    CLASS_BODY_DECLSTAR,            // 34 ClassBodyDeclarations* ClassBodyDeclarations
    CLASS_BODY_DECL_EPSILON,        // 35 ClassBodyDeclarations*
    CLASS_BODY_DECL,                // 36 ClassBodyDeclarations ClassBodyDeclaration
    CLASS_BODY_LIST,                // 37 ClassBodyDeclarations ClassBodyDeclarations ClassBodyDeclaration
    CLASS_MEMBER,                   // 38 ClassBodyDeclaration ClassMemberDeclaration
    CLASS_CONSTRUCTOR,              // 39 ClassBodyDeclaration ConstructorDeclaration
    CLASS_FIELD,                    // 40 ClassMemberDeclaration FieldDeclaration
    CLASS_METHOD,                   // 41 ClassMemberDeclaration MethodDeclaration
    FIELD_DECL,                     // 42 FieldDeclaration MemberModifiers Type VariableDeclarator ;
    MEMBER_MOD,                     // 43 MemberModifiers MemberModifier
    MEMBER_MOD_LIST,                // 44 MemberModifiers MemberModifiers MemberModifier
    MEMBER_MOD_PUBLIC,              // 45 MemberModifier public
    MEMBER_MOD_ABSTRACT,            // 46 MemberModifier abstract
    MEMBER_MOD_STATIC,              // 47 MemberModifier static
    MEMBER_MOD_NATIVE,              // 48 MemberModifier native
    MEMBER_MOD_PROTECTED,           // 49 MemberModifier protected
    MEMBER_MOD_FINAL,               // 50 MemberModifier final
    PRIMITIVE_TYPE,                 // 51 Type PrimitiveType
    REFERENCE_TYPE,                 // 52 Type ReferenceType
    TYPE_BYTE,                      // 53 PrimitiveType byte
    TYPE_SHORT,                     // 54 PrimitiveType short
    TYPE_INT,                       // 55 PrimitiveType int
    TYPE_LONG,                      // 56 PrimitiveType long
    TYPE_CHAR,                      // 57 PrimitiveType char
    TYPE_BOOLEAN,                   // 58 PrimitiveType boolean
    REFERENCE_CLASSINTERFACE,       // 59 ReferenceType ClassOrInterfaceType
    REFERENCE_ARRAY,                // 60 ReferenceType ArrayType
    ARRAY_PRIMITIVE,                // 61 ArrayType PrimitiveType [ ]
    ARRAY_NONPRIMITIVE,             // 62 ArrayType Name [ ]
    DECL_VARIABLE,                  // 63 VariableDeclarator Identifier
    DECL_VARIABLE_ASSIGN,           // 64 VariableDeclarator Identifier = Expression
    EXPRESSION_COND,                // 65 Expression ConditionalExpression
    EXPRESSION_ASSIGN,              // 66 Expression Assignment
    ASSIGNMENT,                     // 67 Assignment LeftHandSide = AssignmentExpression
    LVALUE_NAME,                    // 68 LeftHandSide Name
    LVALUE_FIELD_ACCESS,            // 69 LeftHandSide FieldAccess
    LVALUE_ARRAY,                   // 70 LeftHandSide ArrayAccess
    FIELD_ACCESS,                   // 71 FieldAccess Primary . Identifier
    PRIMARY_NONARRAY,               // 72 Primary PrimaryNoNewArray
    PRIMARY_NEWARRAY,               // 73 Primary ArrayCreationExpression
    PRIMARY_LITERAL,                // 74 PrimaryNoNewArray Literal
    PRIMARY_THIS,                   // 75 PrimaryNoNewArray this
    PRIMARY_EXPRESSION,             // 76 PrimaryNoNewArray ( Expression )
    PRIMARY_MAKECLASS,              // 77 PrimaryNoNewArray ClassInstanceCreationExpression
    PRIMARY_FIELDACCESS,            // 78 PrimaryNoNewArray FieldAccess
    PRIMARY_INVOKE,                 // 79 PrimaryNoNewArray MethodInvocation
    PRIMARY_ARRAY_ACCESS,           // 80 PrimaryNoNewArray ArrayAccess
    LITERAL_NUM,                    // 81 Literal NUMBER
    LITERAL_TRUE,                   // 82 Literal true
    LITERAL_FALSE,                  // 83 Literal false
    LITERAL_CHAR,                   // 84 Literal CHAR
    LITERAL_STRING,                 // 85 Literal STRING
    LITERAL_NULL,                   // 86 Literal null
    MAKE_NEW_CLASS,                 // 87 ClassInstanceCreationExpression new ClassType ( ArgumentList* )
    ARG_LIST,                       // 88 ArgumentList* ArgumentList
    ARG_LIST_EPSILON,               // 89 ArgumentList*
    ARG_LIST_EXPRESSION,            // 90 ArgumentList Expression
    ARG_LIST_LIST,                  // 91 ArgumentList ArgumentList , Expression
    INVOKE_METHOD_NORM,             // 92 MethodInvocation Name ( ArgumentList* )
    INVOKE_METHOD_ACCESS,           // 93 MethodInvocation Primary . Identifier ( ArgumentList* )
    ACCESS_ARRAY_NAME,              // 94 ArrayAccess Name [ Expression ]
    ACCESS_ARRAY_PRIMARY,           // 95 ArrayAccess PrimaryNoNewArray [ Expression ]
    MAKE_NEW_PRIMITIVE_ARRAY,       // 96 ArrayCreationExpression new PrimitiveType DimExpr
    MAKE_NEW_NONPRIMITIVE_ARRAY,    // 97 ArrayCreationExpression new ClassOrInterfaceType DimExpr
    DIM_EXPRESSION,                 // 98 DimExpr [ Expression ]
    ASSIGNEXPR_TO_COND,             // 99 AssignmentExpression ConditionalExpression
    ASSIGNEXPR_TO_ASSIGN,           // 100 AssignmentExpression Assignment
    COND_TO_CONDOR,                 // 101 ConditionalExpression ConditionalOrExpression
    CONDOR_TO_CONDAND,              // 102 ConditionalOrExpression ConditionalAndExpression
    CONDOR_TO_CONDORAND,            // 103 ConditionalOrExpression ConditionalOrExpression || ConditionalAndExpression
    CONDAND_TO_INCLUOR,             // 104 ConditionalAndExpression InclusiveOrExpression
    CONDAND_TO_CONDANDINCLUOR,      // 105 ConditionalAndExpression ConditionalAndExpression && InclusiveOrExpression
    INCLUOR_TO_AND,                 // 106 InclusiveOrExpression AndExpression
    INCLUOR_TO_INCLUORAND,          // 107 InclusiveOrExpression InclusiveOrExpression | AndExpression
    AND_TO_EQUALITY,                // 108 AndExpression EqualityExpression
    AND_TO_ANDEQUALITY,             // 109 AndExpression AndExpression & EqualityExpression
    EQUALITY_TO_RELATIONAL,         // 110 EqualityExpression RelationalExpression
    EQUALITY_TO_EQUALITYRELATION,   // 111 EqualityExpression EqualityExpression == RelationalExpression
    EQUALITY_TO_NOTEQRELATION,      // 112 EqualityExpression EqualityExpression != RelationalExpression
    RELATION_TO_LTRELATIONADD,      // 113 RelationalExpression RelationalExpression < AdditiveExpression
    RELATION_TO_GTRELATIONADD,      // 114 RelationalExpression RelationalExpression > AdditiveExpression
    RELATION_TO_LTERELATIONADD,     // 115 RelationalExpression RelationalExpression <= AdditiveExpression
    RELATION_TO_GTERELATIONADD,     // 116 RelationalExpression RelationalExpression >= AdditiveExpression
    RELATION_TO_INSTANCEOF,         // 117 RelationalExpression RelationalExpression instanceof ReferenceType
    ADD_TO_MULTI,                   // 118 AdditiveExpression MultiplicativeExpression
    ADD_TO_PLUSMULTI,               // 119 AdditiveExpression AdditiveExpression + MultiplicativeExpression
    ADD_TO_MINUSMULTI,              // 120 AdditiveExpression AdditiveExpression - MultiplicativeExpression
    MULTI_TO_UNARY,                 // 121 MultiplicativeExpression UnaryExpression
    MULTI_TO_MULTUNARY,             // 122 MultiplicativeExpression MultiplicativeExpression * UnaryExpression
    MULTI_TO_DIVUNARY,              // 123 MultiplicativeExpression MultiplicativeExpression / UnaryExpression
    MULTI_TO_MODUNARY,              // 124 MultiplicativeExpression MultiplicativeExpression % UnaryExpression
    NEG_UNARY,                      // 125 UnaryExpression - UnaryExpression
    NOT_NEG_UNARY,                  // 126 UnaryExpression UnaryExpressionNotMinus
    NOT_UNARY,                      // 127 UnaryExpressionNotMinus ! UnaryExpression
    UNARY_CAST,                     // 128 UnaryExpressionNotMinus CastExpression
    PRIMARY_UNARY,                  // 129 UnaryExpressionNotMinus Primary
    UNARY_NAME,                     // 130 UnaryExpressionNotMinus Name
    CAST_PRIMITIVE,                 // 131 CastExpression ( PrimitiveType Dims ) UnaryExpression
    CAST_NONPRIMITIVE,              // 132 CastExpression ( Name [ ] ) UnaryExpressionNotMinus
    ARRAY_DIMS,                     // 133 Dims [ ]
    ARRAY_DIMS_EPSILON,             // 134 Dims
    METHOD_HEADER_AND_BODY,         // 135 MethodDeclaration MethodHeader MethodBody
    METHOD_TYPE,                    // 136 MethodHeader MemberModifiers Type MethodDeclarator
    METHOD_VOID,                    // 137 MethodHeader MemberModifiers void MethodDeclarator
    METHOD_DECL,                    // 138 MethodDeclarator Identifier ( FormalParameterList* )
    FORMAL_PARAMSTAR,               // 139 FormalParameterList* FormalParameterList
    FORMAL_PARAMSTAR_EPSILON,       // 140 FormalParameterList*
    FORMAL_PARAM,                   // 141 FormalParameterList FormalParameter
    FORMAL_PARAM_LIST,              // 142 FormalParameterList FormalParameterList , FormalParameter
    PARAMETER_EXPAND,               // 143 FormalParameter Type Identifier
    METHOD_BODY,                    // 144 MethodBody Block
    METHOD_BODY_EMPTY,              // 145 MethodBody ;
    BLOCK,                          // 146 Block { BlockStatements* }
    BLOCK_STMTS,                    // 147 BlockStatements* BlockStatements
    BLOCK_STMTS_EPSILON,            // 148 BlockStatements*
    BLOCK_STMT,                     // 149 BlockStatements BlockStatement
    BLOCK_STMT_LIST,                // 150 BlockStatements BlockStatements BlockStatement
    LOCAL_VAR_STMT,                 // 151 BlockStatement LocalVariableDeclarationStatement
    BLOCK_IS_STMT,                  // 152 BlockStatement Statement
    LOCAL_VAR_DECL,                 // 153 LocalVariableDeclarationStatement LocalVariableDeclaration ;
    TYPE_VAR,                       // 154 LocalVariableDeclaration Type VariableDeclarator
    STMT_NO_TRAILING,               // 155 Statement StatementWithoutTrailingSubstatement
    IF_STMT,                        // 156 Statement IfThenStatement
    IF_THEN_STMT,                   // 157 Statement IfThenElseStatement
    WHILE_STMT,                     // 158 Statement WhileStatement
    FOR_STMT,                       // 159 Statement ForStatement
    STMT_NON_TRAILING_BLOCK,        // 160 StatementWithoutTrailingSubstatement Block
    STMT_NON_TRAILING_EMPTY,        // 161 StatementWithoutTrailingSubstatement EmptyStatement
    STMT_NON_TRAILING_EXPR,         // 162 StatementWithoutTrailingSubstatement ExpressionStatement
    STMT_NON_TRAILING_RETURN,       // 163 StatementWithoutTrailingSubstatement ReturnStatement
    EMPTY,                          // 164 EmptyStatement ;
    EXPR_TO_STMTEXPR,               // 165 ExpressionStatement StatementExpression ;
    STMTEXPR_TO_ASSIGN,             // 166 StatementExpression Assignment
    STMTEXPR_INVOKE,                // 167 StatementExpression MethodInvocation
    STMTEXPR_MAKE_CLASS,            // 168 StatementExpression ClassInstanceCreationExpression
    IF_STMT_UNROLL,                 // 169 IfThenStatement if ( Expression ) Statement
    IF_THEN_STMT_UNROLL,            // 170 IfThenElseStatement if ( Expression ) StatementNoShortIf else Statement
    NOSHORTIF_STMT_NON_TRAILING,    // 171 StatementNoShortIf StatementWithoutTrailingSubstatement
    NO_SHORT_IF_THEN,               // 172 StatementNoShortIf IfThenElseStatementNoShortIf
    NO_SHORT_WHILE,                 // 173 StatementNoShortIf WhileStatementNoShortIf
    NO_SHORT_FOR,                   // 174 StatementNoShortIf ForStatementNoShortIf
    NO_SHORT_IF_THEN_UNROLL,        // 175 IfThenElseStatementNoShortIf if ( Expression ) StatementNoShortIf else StatementNoShortIf
    NO_SHORT_WHILE_UNROLL,          // 176 WhileStatementNoShortIf while ( Expression ) StatementNoShortIf
    NO_SHORT_FOR_UNROLL,            // 177 ForStatementNoShortIf for ( ForInit ; Expression* ; ForUpdate ) StatementNoShortIf
    EXPR_STAR,                      // 178 Expression* Expression
    EXPR_STAR_EPSILON,              // 179 Expression*
    WHILE_UNROLL,                   // 180 WhileStatement while ( Expression ) Statement
    FOR_UNROLL,                     // 181 ForStatement for ( ForInit ; Expression* ; ForUpdate ) Statement
    FOR_INIT_STMT,                  // 182 ForInit StatementExpression
    FOR_INIT_LOCAL_DECL,            // 183 ForInit LocalVariableDeclaration
    FOR_INIT_EMPTY,                 // 184 ForInit
    FOR_UPDATE_STMT,                // 185 ForUpdate StatementExpression
    FOR_UPDATE_EMPTY,               // 186 ForUpdate
    RETURN_UNROLL,                  // 187 ReturnStatement return Expression* ;
    CONSTRUCTOR_PARTS,              // 188 ConstructorDeclaration MemberModifiers ConstructorDeclarator ConstructorBody
    CONSTRUCTOR_DECL,               // 189 ConstructorDeclarator SimpleName ( FormalParameterList* )
    CONSTRUCTOR_BODY_BLOCK,         // 190 ConstructorBody { BlockStatements* }
    INTERFACE_DECL,                 // 191 InterfaceDeclaration Modifiers interface Identifier ExtendsInterfaces InterfaceBody
    EXTENDING,                      // 192 ExtendsInterfaces extends InterfaceType
    EXTENDING_LIST,                 // 193 ExtendsInterfaces ExtendsInterfaces , InterfaceType
    INTERFACE_BODY_BLOCK,           // 194 InterfaceBody { InterfaceMemberDeclarations* }
    INTERFACE_MEMBER_DECL_STAR,     // 195 InterfaceMemberDeclarations* InterfaceMemberDeclarations
    INTERFACE_MEMBER_DECL_EMPTY,    // 196 InterfaceMemberDeclarations*
    INTERFACE_MEMBER_DECL,          // 197 InterfaceMemberDeclarations InterfaceMemberDeclaration
    INTERFACE_MEMBER_DECL_LIST,     // 198 InterfaceMemberDeclarations InterfaceMemberDeclarations InterfaceMemberDeclaration
    INTERFACE_ABSTRACT_METHOD,      // 199 InterfaceMemberDeclaration AbstractMethodDeclaration
    ABSTRACT_METHOD_UNROLL,         // 200 AbstractMethodDeclaration MethodHeader ;
    RELATION_TO_ADDITIVE,           // 201 RelationalExpression AdditiveExpression
    CAST_TO_EXPRESSION,             // 202 CastExpression ( Expression ) UnaryExpressionNotMinus
    EXTENDS_INTERFACE,              // 203 ExtendsInterface* ExtendsInterface
    EXTENDS_INTERFACE_EPSILON,      // 204 ExtendsInterface*
};

#endif
