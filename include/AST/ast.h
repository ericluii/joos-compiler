#ifndef __AST_H__
#define __AST_H__

#include "ruleNumbers.h"

class Ast {
    protected:
        int rule;
    public:
        virtual bool isEpsilon() { return false; }
        virtual ~Ast() {}
        void setRule(int set) { rule = set; }
        int getRule() { return rule; }

        // Imports
        bool isSingleImport() { return rule == SINGLE_TYPE_IMPORT; }
        bool isOnDemandImport() { return rule == IMPORT_ON_DEMAND; }
        // Assignment
        bool isAssignName() { return rule == LVALUE_NAME; }
        bool isAssignField() { return rule == LVALUE_FIELD_ACCESS; }
        bool isAssignArray() { return rule == LVALUE_ARRAY; }
        // Class declarations
        bool isFieldDecl() { return rule == CLASS_FIELD; }
        bool isClassMethodDecl() { return rule == CLASS_METHOD; }
        bool isConstructorDecl() { return rule == CLASS_CONSTRUCTOR; }
        // Literal and this
        bool isThis() { return rule == PRIMARY_THIS; }
        bool isNumber() { return rule == LITERAL_NUM; }
        bool isTrueBoolean() { return rule == LITERAL_TRUE; }
        bool isFalseBoolean() { return rule == LITERAL_FALSE; }
        bool isCharLiteral() { return rule == LITERAL_CHAR; }
        bool isStringLiteral() { return rule == LITERAL_STRING; }
        bool isNull() { return rule == LITERAL_NULL; }
        // Bracketed expression
        bool isPrimaryExpr() { return rule == PRIMARY_EXPRESSION; }
        // Class creation
        bool isNewClassCreation() { return rule == MAKE_NEW_CLASS; }
        // Method invocation
        bool isMethodInvoke() { return rule == PRIMARY_INVOKE; }
        // Variable accessing
        bool isNameAccess() { return rule == UNARY_NAME; }
        bool isFieldAccess() { return rule == FIELD_ACCESS; }
        bool isArrayAccessName() { return rule == ACCESS_ARRAY_NAME; }
        bool isArrayAccessPrimary() { return rule == ACCESS_ARRAY_PRIMARY; }
        // Array creation type
        bool isNewPrimitiveArray() { return rule == MAKE_NEW_PRIMITIVE_ARRAY; }
        bool isNewReferenceArray() { return rule == MAKE_NEW_NONPRIMITIVE_ARRAY; }
        // Type
        bool isPrimitiveType() { return rule == PRIMITIVE_TYPE; }
        bool isReferenceType() { return rule == REFERENCE_TYPE; }
        bool isClassOrInterfaceType() { return rule == REFERENCE_CLASSINTERFACE; }
        bool isPrimitiveArrayType() { return rule == ARRAY_PRIMITIVE; }
        bool isReferenceArrayType() { return rule == ARRAY_NONPRIMITIVE; }
        bool isTypeInt() { return rule == TYPE_INT; }
        bool isTypeShort() { return rule == TYPE_SHORT; }
        bool isTypeByte() { return rule == TYPE_BYTE; }
        bool isTypeChar() { return rule == TYPE_CHAR; }
        bool isTypeBoolean() { return rule == TYPE_BOOLEAN; }
        // Class member
        bool isField() { return rule == CLASS_FIELD; }
        bool isMethod() { return rule == CLASS_METHOD; }
        bool isConstructor() { return rule == CLASS_CONSTRUCTOR; }
        // Method invocation
        bool isNormalMethodCall() { return rule == INVOKE_METHOD_NORM; }
        bool isAccessedMethodCall() { return rule == INVOKE_METHOD_ACCESS; }
        // Block statements
        bool isLastBlock() { return rule == BLOCK_STMT; }
        bool isMoreBlocksComing() { return rule == BLOCK_STMT_LIST; }
        bool isLocalVarDecl() { return rule == LOCAL_VAR_DECL; }
        bool isIfStmt() { return rule == IF_STMT_UNROLL; }
        bool isIfThenElseStmt() { return rule == IF_STMT || rule == IF_THEN_STMT || rule == NO_SHORT_IF_THEN; }
        bool isWhileStmt() { return rule == WHILE_STMT || rule == NO_SHORT_WHILE; }
        bool isForStmt() { return rule == FOR_STMT || rule == NO_SHORT_FOR; }
        bool isAssignStmt() { return rule == STMTEXPR_TO_ASSIGN; }
        bool isClassCreationStmt() { return rule == STMTEXPR_MAKE_CLASS; }
        bool isMethodInvokeStmt() { return rule == STMTEXPR_INVOKE; }
        bool isNestedBlock() { return rule == STMT_NON_TRAILING_BLOCK; }
        bool isReturnStmt() { return rule == STMT_NON_TRAILING_RETURN; }
        bool isEmptyStmt() { return rule == STMT_NON_TRAILING_EMPTY; }
};

#endif
