#ifndef __RULENUMBERS_H__
#define __RULENUMBERS_H__

// Rules

enum RuleNumbers {
    GOAL = 0,                       // 0 Goal CompilationUnit
    COMPILATION_UNIT,               // 1 CompilationUnit PackageDeclaration ImportDeclarations* TypeDeclaration $
    TYPE_CLASS,                     // 2 TypeDeclaration ClassDeclaration
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
    TYPE_CHAR,                      // 56 PrimitiveType char
    TYPE_BOOLEAN,                   // 57 PrimitiveType boolean
    REFERENCE_CLASSINTERFACE,       // 58 ReferenceType ClassOrInterfaceType
    REFERENCE_ARRAY,                // 59 ReferenceType ArrayType
    ARRAY_PRIMITIVE,                // 60 ArrayType PrimitiveType [ ]
    ARRAY_NONPRIMITIVE,             // 61 ArrayType Name [ ]
    DECL_VARIABLE,                  // 62 VariableDeclarator Identifier
    DECL_VARIABLE_ASSIGN,           // 63 VariableDeclarator Identifier = Expression
    EXPRESSION_COND,                // 64 Expression ConditionalExpression
    EXPRESSION_ASSIGN,              // 65 Expression Assignment
    ASSIGNMENT,                     // 66 Assignment LeftHandSide = AssignmentExpression
    LVALUE_NAME,                    // 67 LeftHandSide Name
    LVALUE_FIELD_ACCESS,            // 68 LeftHandSide FieldAccess
    LVALUE_ARRAY,                   // 69 LeftHandSide ArrayAccess
    FIELD_ACCESS,                   // 70 FieldAccess Primary . Identifier
    PRIMARY_NONARRAY,               // 71 Primary PrimaryNoNewArray
    PRIMARY_NEWARRAY,               // 72 Primary ArrayCreationExpression
    PRIMARY_LITERAL,                // 73 PrimaryNoNewArray Literal
    PRIMARY_THIS,                   // 74 PrimaryNoNewArray this
    PRIMARY_EXPRESSION,             // 75 PrimaryNoNewArray ( Expression )
    PRIMARY_MAKECLASS,              // 76 PrimaryNoNewArray ClassInstanceCreationExpression
    PRIMARY_FIELDACCESS,            // 77 PrimaryNoNewArray FieldAccess
    PRIMARY_INVOKE,                 // 78 PrimaryNoNewArray MethodInvocation
    PRIMARY_ARRAY_ACCESS,           // 79 PrimaryNoNewArray ArrayAccess
    LITERAL_NUM,                    // 80 Literal NUMBER
    LITERAL_TRUE,                   // 81 Literal true
    LITERAL_FALSE,                  // 82 Literal false
    LITERAL_CHAR,                   // 83 Literal CHAR
    LITERAL_STRING,                 // 84 Literal STRING
    LITERAL_NULL,                   // 85 Literal null
    MAKE_NEW_CLASS,                 // 86 ClassInstanceCreationExpression new ClassType ( ArgumentList* )
    ARG_LIST,                       // 87 ArgumentList* ArgumentList
    ARG_LIST_EPSILON,               // 88 ArgumentList*
    ARG_LIST_EXPRESSION,            // 89 ArgumentList Expression
    ARG_LIST_LIST,                  // 90 ArgumentList ArgumentList , Expression
    INVOKE_METHOD_NORM,             // 91 MethodInvocation Name ( ArgumentList* )
    INVOKE_METHOD_ACCESS,           // 92 MethodInvocation FieldAccess ( ArgumentList* )
    ACCESS_ARRAY_NAME,              // 93 ArrayAccess Name [ Expression ]
    ACCESS_ARRAY_PRIMARY,           // 94 ArrayAccess PrimaryNoNewArray [ Expression ]
    MAKE_NEW_PRIMITIVE_ARRAY,       // 95 ArrayCreationExpression new PrimitiveType DimExpr
    MAKE_NEW_NONPRIMITIVE_ARRAY,    // 96 ArrayCreationExpression new ClassOrInterfaceType DimExpr
    DIM_EXPRESSION,                 // 97 DimExpr [ Expression ]
    ASSIGNEXPR_TO_COND,             // 98 AssignmentExpression ConditionalExpression
    ASSIGNEXPR_TO_ASSIGN,           // 99 AssignmentExpression Assignment
    COND_TO_CONDOR,                 // 100 ConditionalExpression ConditionalOrExpression
    CONDOR_TO_CONDAND,              // 101 ConditionalOrExpression ConditionalAndExpression
    CONDOR_TO_CONDORAND,            // 102 ConditionalOrExpression ConditionalOrExpression || ConditionalAndExpression
    CONDAND_TO_INCLUOR,             // 103 ConditionalAndExpression InclusiveOrExpression
    CONDAND_TO_CONDANDINCLUOR,      // 104 ConditionalAndExpression ConditionalAndExpression && InclusiveOrExpression
    INCLUOR_TO_AND,                 // 105 InclusiveOrExpression AndExpression
    INCLUOR_TO_INCLUORAND,          // 106 InclusiveOrExpression InclusiveOrExpression | AndExpression
    AND_TO_EQUALITY,                // 107 AndExpression EqualityExpression
    AND_TO_ANDEQUALITY,             // 108 AndExpression AndExpression & EqualityExpression
    EQUALITY_TO_RELATIONAL,         // 109 EqualityExpression RelationalExpression
    EQUALITY_TO_EQUALITYRELATION,   // 110 EqualityExpression EqualityExpression == RelationalExpression
    EQUALITY_TO_NOTEQRELATION,      // 111 EqualityExpression EqualityExpression != RelationalExpression
    RELATION_TO_LTRELATIONADD,      // 112 RelationalExpression RelationalExpression < AdditiveExpression
    RELATION_TO_GTRELATIONADD,      // 113 RelationalExpression RelationalExpression > AdditiveExpression
    RELATION_TO_LTERELATIONADD,     // 114 RelationalExpression RelationalExpression <= AdditiveExpression
    RELATION_TO_GTERELATIONADD,     // 115 RelationalExpression RelationalExpression >= AdditiveExpression
    RELATION_TO_INSTANCEOF,         // 116 RelationalExpression RelationalExpression instanceof ReferenceType
    ADD_TO_MULTI,                   // 117 AdditiveExpression MultiplicativeExpression
    ADD_TO_PLUSMULTI,               // 118 AdditiveExpression AdditiveExpression + MultiplicativeExpression
    ADD_TO_MINUSMULTI,              // 119 AdditiveExpression AdditiveExpression - MultiplicativeExpression
    MULTI_TO_UNARY,                 // 120 MultiplicativeExpression UnaryExpression
    MULTI_TO_MULTUNARY,             // 121 MultiplicativeExpression MultiplicativeExpression * UnaryExpression
    MULTI_TO_DIVUNARY,              // 122 MultiplicativeExpression MultiplicativeExpression / UnaryExpression
    MULTI_TO_MODUNARY,              // 123 MultiplicativeExpression MultiplicativeExpression % UnaryExpression
    NEG_UNARY,                      // 124 UnaryExpression - UnaryExpression
    NOT_NEG_UNARY,                  // 125 UnaryExpression UnaryExpressionNotMinus
    NOT_UNARY,                      // 126 UnaryExpressionNotMinus ! UnaryExpression
    UNARY_CAST,                     // 127 UnaryExpressionNotMinus CastExpression
    PRIMARY_UNARY,                  // 128 UnaryExpressionNotMinus Primary
    UNARY_NAME,                     // 129 UnaryExpressionNotMinus Name
    CAST_PRIMITIVE,                 // 130 CastExpression ( PrimitiveType Dims ) UnaryExpression
    CAST_NONPRIMITIVE,              // 131 CastExpression ( Name [ ] ) UnaryExpressionNotMinus
    ARRAY_DIMS,                     // 132 Dims [ ]
    ARRAY_DIMS_EPSILON,             // 133 Dims
    METHOD_HEADER_AND_BODY,         // 134 MethodDeclaration MethodHeader MethodBody
    METHOD_TYPE,                    // 135 MethodHeader MemberModifiers Type MethodDeclarator
    METHOD_VOID,                    // 136 MethodHeader MemberModifiers void MethodDeclarator
    METHOD_DECL,                    // 137 MethodDeclarator Identifier ( FormalParameterList* )
    FORMAL_PARAMSTAR,               // 138 FormalParameterList* FormalParameterList
    FORMAL_PARAMSTAR_EPSILON,       // 139 FormalParameterList*
    FORMAL_PARAM,                   // 140 FormalParameterList FormalParameter
    FORMAL_PARAM_LIST,              // 141 FormalParameterList FormalParameterList , FormalParameter
    PARAMETER_EXPAND,               // 142 FormalParameter Type Identifier
    METHOD_BODY,                    // 143 MethodBody Block
    METHOD_BODY_EMPTY,              // 144 MethodBody ;
    BLOCK,                          // 145 Block { BlockStatements* }
    BLOCK_STMTS,                    // 146 BlockStatements* BlockStatements
    BLOCK_STMTS_EPSILON,            // 147 BlockStatements*
    BLOCK_STMT,                     // 148 BlockStatements BlockStatement
    BLOCK_STMT_LIST,                // 149 BlockStatements BlockStatements BlockStatement
    LOCAL_VAR_STMT,                 // 150 BlockStatement LocalVariableDeclarationStatement
    BLOCK_IS_STMT,                  // 151 BlockStatement Statement
    LOCAL_VAR_DECL,                 // 152 LocalVariableDeclarationStatement LocalVariableDeclaration ;
    TYPE_VAR,                       // 153 LocalVariableDeclaration Type Identifier = Expression
    STMT_NO_TRAILING,               // 154 Statement StatementWithoutTrailingSubstatement
    IF_STMT,                        // 155 Statement IfThenStatement
    IF_THEN_STMT,                   // 156 Statement IfThenElseStatement
    WHILE_STMT,                     // 157 Statement WhileStatement
    FOR_STMT,                       // 158 Statement ForStatement
    STMT_NON_TRAILING_BLOCK,        // 159 StatementWithoutTrailingSubstatement Block
    STMT_NON_TRAILING_EMPTY,        // 160 StatementWithoutTrailingSubstatement EmptyStatement
    STMT_NON_TRAILING_EXPR,         // 161 StatementWithoutTrailingSubstatement ExpressionStatement
    STMT_NON_TRAILING_RETURN,       // 162 StatementWithoutTrailingSubstatement ReturnStatement
    EMPTY,                          // 163 EmptyStatement ;
    EXPR_TO_STMTEXPR,               // 164 ExpressionStatement StatementExpression ;
    STMTEXPR_TO_ASSIGN,             // 165 StatementExpression Assignment
    STMTEXPR_INVOKE,                // 166 StatementExpression MethodInvocation
    STMTEXPR_MAKE_CLASS,            // 167 StatementExpression ClassInstanceCreationExpression
    IF_STMT_UNROLL,                 // 168 IfThenStatement if ( Expression ) Statement
    IF_THEN_STMT_UNROLL,            // 169 IfThenElseStatement if ( Expression ) StatementNoShortIf else Statement
    NOSHORTIF_STMT_NON_TRAILING,    // 170 StatementNoShortIf StatementWithoutTrailingSubstatement
    NO_SHORT_IF_THEN,               // 171 StatementNoShortIf IfThenElseStatementNoShortIf
    NO_SHORT_WHILE,                 // 172 StatementNoShortIf WhileStatementNoShortIf
    NO_SHORT_FOR,                   // 173 StatementNoShortIf ForStatementNoShortIf
    NO_SHORT_IF_THEN_UNROLL,        // 174 IfThenElseStatementNoShortIf if ( Expression ) StatementNoShortIf else StatementNoShortIf
    NO_SHORT_WHILE_UNROLL,          // 175 WhileStatementNoShortIf while ( Expression ) StatementNoShortIf
    NO_SHORT_FOR_UNROLL,            // 176 ForStatementNoShortIf for ( ForInit ; Expression* ; ForUpdate ) StatementNoShortIf
    EXPR_STAR,                      // 177 Expression* Expression
    EXPR_STAR_EPSILON,              // 178 Expression*
    WHILE_UNROLL,                   // 179 WhileStatement while ( Expression ) Statement
    FOR_UNROLL,                     // 180 ForStatement for ( ForInit ; Expression* ; ForUpdate ) Statement
    FOR_INIT_STMT,                  // 181 ForInit StatementExpression
    FOR_INIT_LOCAL_DECL,            // 182 ForInit LocalVariableDeclaration
    FOR_INIT_EMPTY,                 // 183 ForInit
    FOR_UPDATE_STMT,                // 184 ForUpdate StatementExpression
    FOR_UPDATE_EMPTY,               // 185 ForUpdate
    RETURN_UNROLL,                  // 186 ReturnStatement return Expression* ;
    CONSTRUCTOR_PARTS,              // 187 ConstructorDeclaration MemberModifiers ConstructorDeclarator ConstructorBody
    CONSTRUCTOR_DECL,               // 188 ConstructorDeclarator SimpleName ( FormalParameterList* )
    CONSTRUCTOR_BODY_BLOCK,         // 189 ConstructorBody { BlockStatements* }
    INTERFACE_DECL,                 // 190 InterfaceDeclaration Modifiers interface Identifier ExtendsInterfaces* InterfaceBody
    EXTENDING,                      // 191 ExtendsInterfaces InterfaceType
    EXTENDING_LIST,                 // 192 ExtendsInterfaces ExtendsInterfaces , InterfaceType
    INTERFACE_BODY_BLOCK,           // 193 InterfaceBody { InterfaceMemberDeclarations* }
    INTERFACE_MEMBER_DECL_STAR,     // 194 InterfaceMemberDeclarations* InterfaceMemberDeclarations
    INTERFACE_MEMBER_DECL_EMPTY,    // 195 InterfaceMemberDeclarations*
    INTERFACE_MEMBER_DECL,          // 196 InterfaceMemberDeclarations InterfaceMemberDeclaration
    INTERFACE_MEMBER_DECL_LIST,     // 197 InterfaceMemberDeclarations InterfaceMemberDeclarations InterfaceMemberDeclaration
    INTERFACE_ABSTRACT_METHOD,      // 198 InterfaceMemberDeclaration AbstractMethodDeclaration
    ABSTRACT_METHOD_UNROLL,         // 199 AbstractMethodDeclaration AbstractMethodHeader ;
    RELATION_TO_ADDITIVE,           // 200 RelationalExpression AdditiveExpression
    CAST_TO_EXPRESSION,             // 201 CastExpression ( Expression ) UnaryExpressionNotMinus
    EXTENDS_INTERFACE,              // 202 ExtendsInterface* extends ExtendsInterface
    EXTENDS_INTERFACE_EPSILON,      // 203 ExtendsInterface*
    TYPE_INTERFACE,                 // 204 TypeDeclaration InterfaceDeclaration
    TYPE_EPSILON,                   // 205 TypeDeclaration
    MEMBER_MOD_EXPAND,              // 206 MemberModifiers* MemberModifiers
    MEMBER_MOD_EPSILON,             // 207 MemberModifiers*
    ABSTRACT_METHOD_TYPE,           // 208 AbstractMethodHeader MemberModifiers* Type MethodDeclarator
    ABSTRACT_METHOD_VOID,           // 209 AbstractMethodHeader MemberModifiers* void MethodDeclarator
};

#endif
