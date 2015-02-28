#ifndef __HIERARCHY_CHECKING__
#define __HIERARCHY_CHECKING__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"

class HierarchyChecking {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;

        // Checks
        void classNotExtendInterface();
    public:
        HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages);

        void check();
};

#endif
