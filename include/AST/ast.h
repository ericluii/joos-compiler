#ifndef __AST_H__
#define __AST_H__

#include <string>
#include "ruleNumbers.h"

class Ast {
    protected:
        int rule;
        std::string treeLexeme;
    public:
        virtual bool isEpsilon() { return false; }
        virtual ~Ast() {}
        void setRuleAndLexeme(int setRule, const std::string& setLexeme) { 
            rule = setRule;
            treeLexeme = setLexeme;
        }
        int getRule() { return rule; }
        std::string& getLexeme() { return treeLexeme; }

        // Compilation unit
        bool isCompilationUnit() { return rule == COMPILATION_UNIT; }
        // Package
        bool isPackageDecl() { return rule == PACKAGE_NAME || rule == PACKAGE_EPSILON; }
        // Imports
        bool isImportDeclsStar() { return rule == IMPORT_STAR_DECLS || rule == IMPORT_STAR_EPSILON; }
        bool isSingleImport() { return rule == SINGLE_TYPE_IMPORT; }
        bool isOnDemandImport() { return rule == IMPORT_ON_DEMAND; }
        // Extends class
        bool isSuper() { return rule == EXTENDS_CLASS || rule == EXTENDS_CLASS_EPSILON; }
        // Implements interface
        bool isImplementsInterfaces() { return rule == IMPLEMENTING || rule == IMPLEMENT_EPSILON; }
        // Extends interface
        bool isExtendsInterfaces() { return rule == EXTENDS_INTERFACE || rule == EXTENDS_INTERFACE_EPSILON; }
        // Class or interface type
        bool isClass() { return rule == TYPE_CLASS; }
        bool isInterface() { return rule == TYPE_INTERFACE; }
        bool isNoTypeDefined() { return rule == TYPE_EPSILON; }
        // Name
        bool isSimpleName() { return rule == NAME_SIMPLE; }
        bool isQualifiedName() { return rule == NAME_QUALIFIED; }
        // Identifier
        bool isIdentifier() { return rule == IDENTIFIER; }
        // Modifiers
        bool isModifiersStar() { return rule == MEMBER_MOD_EXPAND || rule == MEMBER_MOD_EPSILON; }
        bool isModifiers() { return rule == MODIFIERS_MOD || rule == MEMBER_MOD ||
                                    rule == MODIFIERS_LIST || rule == MEMBER_MOD_LIST; }
        // Assignment
        bool isAssignName() { return rule == LVALUE_NAME; }
        bool isAssignField() { return rule == LVALUE_FIELD_ACCESS; }
        bool isAssignArray() { return rule == LVALUE_ARRAY; }
        // Expression
        bool isExpressionStar() { return rule == EXPR_STAR || rule == EXPR_STAR_EPSILON; }
        bool isLazyOr() { return rule == CONDOR_TO_CONDORAND; }
        bool isLazyAnd() { return rule == CONDAND_TO_CONDANDINCLUOR; }
        bool isEagerOr() { return rule == INCLUOR_TO_INCLUORAND; }
        bool isEagerAnd() { return rule == AND_TO_ANDEQUALITY; }
        bool isEqual() { return rule == EQUALITY_TO_EQUALITYRELATION; }
        bool isNotEqual() { return rule == EQUALITY_TO_NOTEQRELATION; }
        bool isLT() { return rule == RELATION_TO_LTRELATIONADD; }
        bool isGT() { return rule == RELATION_TO_GTRELATIONADD; }
        bool isLTE() { return rule == RELATION_TO_LTERELATIONADD; }
        bool isGTE() { return rule == RELATION_TO_GTERELATIONADD; }
        bool isInstanceOf() { return rule == RELATION_TO_INSTANCEOF; }
        bool isAddition() { return rule == ADD_TO_PLUSMULTI; }
        bool isMinus() { return rule == ADD_TO_MINUSMULTI; }
        bool isMultiplication() { return rule == MULTI_TO_MULTUNARY; }
        bool isDivision() { return rule == MULTI_TO_DIVUNARY; }
        bool isModulo() { return rule == MULTI_TO_MODUNARY; }
        bool isPrimaryExpression() { return rule == PRIMARY_UNARY; }
        bool isNameExpression() { return rule == UNARY_NAME; }
        bool isNumericNegation() { return rule == NEG_UNARY; }
        bool isBooleanNegation() { return rule == NOT_UNARY; }
        bool isCastToReferenceType() { return rule == CAST_NONPRIMITIVE; }
        bool isCastToArrayName() { return rule == CAST_TO_EXPRESSION; }
        bool isCastToPrimitiveType() { return rule == CAST_PRIMITIVE; }
        // Class declarations
        bool isFieldDecl() { return rule == CLASS_FIELD; }
        bool isClassMethodDecl() { return rule == CLASS_METHOD; }
        bool isConstructorDecl() { return rule == CLASS_CONSTRUCTOR; }
        // Interface methods
        bool isInterfaceMethod() { return rule == INTERFACE_MEMBER_DECL || rule == INTERFACE_MEMBER_DECL_LIST; }
        // Literal and this
        bool isThis() { return rule == PRIMARY_THIS; }
        bool isNumber() { return rule == LITERAL_NUM; }
        bool isTrueBoolean() { return rule == LITERAL_TRUE; }
        bool isFalseBoolean() { return rule == LITERAL_FALSE; }
        bool isCharLiteral() { return rule == LITERAL_CHAR; }
        bool isStringLiteral() { return rule == LITERAL_STRING; }
        bool isNull() { return rule == LITERAL_NULL; }
        // Qualified this
        bool isQualifiedThis() { return rule == PRIMARY_QUALIFIED_THIS; }
        // Bracketed expression
        bool isBracketedExpression() { return rule == PRIMARY_EXPRESSION; }
        // Class creation
        bool isNewClassCreation() { return rule == MAKE_NEW_CLASS; }
        // Variable accessing
        bool isNameAccess() { return rule == UNARY_NAME; }
        bool isFieldAccess() { return rule == FIELD_ACCESS; }
        bool isArrayAccessName() { return rule == ACCESS_ARRAY_NAME; }
        bool isArrayAccessPrimary() { return rule == ACCESS_ARRAY_PRIMARY; }
        // Array creation type
        bool isNewPrimitiveArray() { return rule == MAKE_NEW_PRIMITIVE_ARRAY; }
        bool isNewReferenceArray() { return rule == MAKE_NEW_NONPRIMITIVE_ARRAY; }
        // Type
        bool isPrimitiveType() { return rule == TYPE_INT || rule == TYPE_SHORT || rule == TYPE_BYTE ||
                                        rule == TYPE_CHAR || rule == TYPE_BOOLEAN; }
        bool isReferenceType() { return rule == REFERENCE_CLASSINTERFACE || rule == ARRAY_NONPRIMITIVE; }
        bool isClassOrInterfaceType() { return rule == REFERENCE_CLASSINTERFACE; }
        bool isReferenceArrayType() { return rule == ARRAY_NONPRIMITIVE; }
        bool isTypeInt() { return rule == TYPE_INT; }
        bool isTypeShort() { return rule == TYPE_SHORT; }
        bool isTypeByte() { return rule == TYPE_BYTE; }
        bool isTypeChar() { return rule == TYPE_CHAR; }
        bool isTypeBoolean() { return rule == TYPE_BOOLEAN; }
        // Class body star
        bool isClassBodyStar() { return rule == CLASS_BODY_DECLSTAR || rule == CLASS_BODY_DECL_EPSILON; }
        // Class member
        bool isField() { return rule == CLASS_FIELD; }
        bool isClassMethod() { return rule == METHOD_HEADER_AND_BODY; }
        bool isConstructor() { return rule == CLASS_CONSTRUCTOR; }
        // Method invocation
        bool isNormalMethodCall() { return rule == INVOKE_METHOD_NORM; }
        bool isAccessedMethodCall() { return rule == INVOKE_METHOD_ACCESS; }
        // Method body
        bool isMethodBody() { return rule == METHOD_BODY || rule == METHOD_BODY_EMPTY; }
        // Block statements star
        bool isBlockStmtsStar() { return rule == BLOCK_STMTS || rule == BLOCK_STMTS_EPSILON; }
        // Block statements
        bool isLocalVarDecl() { return rule == TYPE_VAR; }
        bool isIfStmt() { return rule == IF_STMT_UNROLL; }
        bool isIfThenElseStmt() { return rule == IF_THEN_STMT_UNROLL || rule == NO_SHORT_IF_THEN_UNROLL; }
        bool isWhileStmt() { return rule == WHILE_UNROLL || rule == NO_SHORT_WHILE_UNROLL; }
        bool isForStmt() { return rule == FOR_UNROLL || rule == NO_SHORT_FOR_UNROLL; }
        bool isAssignStmt() { return rule == STMTEXPR_TO_ASSIGN; }
        bool isClassCreationStmt() { return rule == STMTEXPR_MAKE_CLASS; }
        bool isMethodInvokeStmt() { return rule == STMTEXPR_INVOKE; }
        bool isNestedBlock() { return rule == STMT_NON_TRAILING_BLOCK; }
        bool isReturnStmt() { return rule == STMT_NON_TRAILING_RETURN; }
        bool isEmptyStmt() { return rule == STMT_NON_TRAILING_EMPTY; }
};

#endif
