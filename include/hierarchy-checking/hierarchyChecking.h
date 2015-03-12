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
        CompilationTable* object;

        CompilationTable* retrieveCompilationOfTypeName(CompilationTable* compilation, Name* typeName, Token* token);

        // Checks
        void classNotExtendInterface(CompilationTable* compilation);
        void duplicateInterface(CompilationTable* compilation);
        void interfaceNotExtendClass(CompilationTable* compilation);
        void noDuplicateSignature(CompilationTable* compilation);
        void OverrideChecks(CompilationTable* compilation);
        void classNotImplementClass(CompilationTable* compilation);
        void classNotExtendFinalClass(CompilationTable* compilation);
        void checkMethodModifiers(CompilationTable* compilation);
        void checkForCycles(CompilationTable* compilation);

        // To be called after checks
        // Bring down inherited methods and fields from superclass (only for classes)
        void establishInheritance(CompilationTable* compilation);
    public:
        HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
