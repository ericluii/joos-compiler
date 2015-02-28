#include "hierarchyChecking.h"
#include "compilationUnit.h"
#include "error.h"

HierarchyChecking::HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

void HierarchyChecking::classNotExtendInterface() {
    // Do stuff
}

void HierarchyChecking::check() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for (it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            // PLACE CHECKS HERE
            
            
            if (Error::count() > 0) { return; }
        }
    }
}
