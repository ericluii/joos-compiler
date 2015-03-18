#ifndef __TYPE_CHECKER_H__
#define __TYPE_CHECKER_H__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"
#include "classDecl.h"
#include "classBodyStar.h"
#include "classBodyDecls.h"
#include "classMethod.h"
#include "constructor.h"
#include "methodBody.h"
#include "blockStmtsStar.h"
#include "blockStmts.h"
#include "localDecl.h"
#include "interfaceDecl.h"
#include "interfaceTable.h"
#include "returnStmt.h"
#include "stmtExprAssign.h"
#include "assignment.h"
#include "assignField.h"
#include "assignArray.h"
#include "assignName.h"
#include "stmtExprCreation.h"
#include "stmtExprInvoke.h"
#include "methodInvoke.h"
#include "methodNormalInvoke.h"
#include "invokeAccessedMethod.h"
#include "bracketedExpression.h"
#include "packagesManager.h"
#include "fieldTable.h"
#include "fieldAccess.h"
#include "fieldDecl.h"
#include "newClassCreation.h"
#include "arrayAccessName.h"
#include "binaryExpression.h"
#include "primaryExpression.h"
#include "ifStmt.h"
#include "whileStmt.h"
#include "nameExpression.h"
#include "forStmt.h"
#include "nestedBlock.h"
#include "expressionStar.h"
#include "primaryNewArray.h"
#include "qualifiedThis.h"
#include "literalOrThis.h"
#include "negationExpression.h"
#include "instanceof.h"
#include "arrayAccessPrimary.h"
#include "castExpression.h"
#include "castPrimitive.h"
#include <stack>

enum ST_TYPE {
    CLASS_TABLE,
    CLASSMETHOD_TABLE,
    CONSTRUCTOR_TABLE,
    FIELDDECL_TABLE,
    NONE
};

#define CHECK_PUSH(expr, symboltable, symbol_table_type) st_stack.push(symboltable);\
    ST_TYPE last_type = cur_st_type;\
    cur_st_type = symbol_table_type;\
    bool ra = check(expr);\
    cur_st_type = last_type;\
    st_stack.pop();\
    return ra;

#define CHECK_PUSH_AND_SET(expr, symboltable, symbol_table_type, extra_bool, condition) st_stack.push(symboltable);\
    ST_TYPE last_type = cur_st_type;\
    cur_st_type = symbol_table_type;\
    extra_bool = condition;\
    bool ra = check(expr);\
    cur_st_type = last_type;\
    extra_bool = false;\
    st_stack.pop();\
    return ra;

class TypeChecking {
    private:
        PackagesManager& manager;
        std::map<std::string, std::vector<CompilationTable*> >& packages;
        CompilationTable* processing;
        std::stack<SymbolTable*> st_stack;

        ST_TYPE cur_st_type;

        bool restrict_this;
        bool restrict_null;
        bool restrict_num;
        bool restrict_type_name_expressions;

        bool static_context_only;
        bool numeric_expression_only;
        bool boolean_expression_only;

        bool check(CompilationTable* compilation);
        bool check(ClassDecl* classDecl);
        bool check(ClassBodyStar* classBodyStar);
        bool check(ClassBodyDecls* classBodyDecls);
        bool check(ClassMethod* classMethod);
        bool check(Constructor* constructor);
        bool check(MethodBody* methodBody);
        bool check(BlockStmtsStar* blockStmtsStar);
        bool check(BlockStmts* blockStmts);
        bool check(LocalDecl* localDecl);
        bool check(ReturnStmt* returnStmt);
        bool check(Assignment* assignment);
        bool check(StmtExprCreation* stmtExprCreation);
        bool check(MethodInvoke* methodInvoke);
        bool check(NewClassCreation* newClassCreation);
        bool check(Expression* expression);
        bool check(ExpressionStar* expressionStar);
        bool check(Primary* primary);
        bool check(FieldDecl* fieldDecl);
        bool check(IfStmt* ifStmt);
        bool check(WhileStmt* whileStmt);
        bool check(ForStmt* forStmt);
        bool check(NestedBlock* nestedBlock);
        bool check(StmtExpr* stmtExpr);
        bool check(LiteralOrThis* literalOrThis);
        bool check(QualifiedThis* qualifiedThis);
        bool check(InstanceOf* instanceOf);
        bool check(FieldAccess* fieldAccess);
        bool check(NameExpression* nameExpression);
        bool check(ArrayAccess* arrayAccess);
        bool check(PrimaryNewArray* primaryNewArray);
        bool check(CastExpression* castExpression);
        bool check(NegationExpression* negationExpression);

        bool isLocalOrArg(Name* name);
        bool inheritsOrExtendsOrImplements(std::string classname, std::string searchname);
        bool assignmentCheck(std::string lefths, Expression* expr);
        std::string tryToGetTypename(Name* name, CompilationTable* cur_table);
    public:
        TypeChecking(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
