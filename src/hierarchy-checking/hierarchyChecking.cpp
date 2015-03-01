#include "hierarchyChecking.h"
#include "compilationUnit.h"
#include "error.h"
#include "ClassDecl.h"
#include "name.h"

HierarchyChecking::HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

void HierarchyChecking::classNotExtendInterface(CompilationTable* compilation) {
    if (compilation->isClassSymbolTable()) {
        SymbolTable* st = compilation->getSymbolTable();

        if (st->isClassTable()) {
            ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();

            if (!cd->noSuperClass()) {
                Name* name = cd->getSuper()->getSuperName();
                //std::string packageName = name->
                //for (unsigned int i = 0; i < packages[name->getQualifier()]
            }
        }
    }
}

void HierarchyChecking::check() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for (it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            // PLACE CHECKS HERE
     //       classNotExtendInterface(*it2);

            if (Error::count() > 0) { return; }
        }
    }
}
