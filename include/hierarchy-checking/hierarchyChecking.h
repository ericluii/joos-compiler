#ifndef __HIERARCHY_CHECKING__
#define __HIERARCHY_CHECKING__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"
#include "name.h"
#include "token.h"

class HierarchyChecking {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;

        CompilationTable* retrieveCompilationOfTypeName(CompilationTable* compilation, Name* typeName, Token* token);

        // Checks
        void classNotExtendInterface(CompilationTable* compilation);
        void duplicateInterface(CompilationTable* compilation);
        void interfaceNotExtendClass(CompilationTable* compilation);
        void noDuplicateSignature(CompilationTable* compilation);
        void NoStaticOverride(CompilationTable* compilation);
        void classNotImplementClass(CompilationTable* compilation);
        void classNotExtendFinalClass(CompilationTable* compilation);
        void checkMethodModifiers(CompilationTable* compilation);
    public:
        HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
