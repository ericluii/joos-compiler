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

class TypeChecking {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;

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

        bool inheritsOrExtendsOrImplements(std::string classname, std::string searchname);
    public:
        TypeChecking(std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
