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
#include "packagesManager.h"
#include "fieldTable.h"
#include "fieldDecl.h"
#include "newClassCreation.h"
#include <stack>

class TypeChecking {
    private:
        PackagesManager& manager;
        std::map<std::string, std::vector<CompilationTable*> >& packages;
        CompilationTable* processing;
        std::stack<SymbolTable*> st_stack;

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

        bool inheritsOrExtendsOrImplements(std::string classname, std::string searchname);
        bool assignmentCheck(std::string lefths, Expression* expr);
        std::string tryToGetTypename(Name* name, CompilationTable* cur_table);
    public:
        TypeChecking(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
