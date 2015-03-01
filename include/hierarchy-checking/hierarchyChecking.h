#ifndef __HIERARCHY_CHECKING__
#define __HIERARCHY_CHECKING__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"
#include "name.h"

class HierarchyChecking {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;

        CompilationTable* retrieveCompilationOfTypeName(CompilationTable* compilation, Name* typeName);

        // Checks
        void classNotExtendInterface(CompilationTable* compilation);
        void duplicateInterface(CompilationTable* compilation);
    public:
        HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
