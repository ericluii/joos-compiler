#ifndef __TYPELINKER_H__
#define __TYPELINKER_H__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"

class ImportDecls;

class TypeLinker {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;
        // Related to single type imports clashing with types in the same file
        void checkSingleImportClashError(const std::string& typeName, ImportDecls* imports);
        void checkImportsClashWithType(const std::string& typeName, ImportDecls* imports);
        void checkForClashingSingleImportInFile(CompilationTable*);

        // Related to checking for imported types or packages to exist
        void setImportIfExist(CompilationTable* compilation, ImportDecls* import);
        void importsExistenceRecurse(CompilationTable* compilation, ImportDecls* import);
        void checkImportsExistenceAndSet(CompilationTable* compilation);
    public:
        TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages);
        ~TypeLinker();

        void typeLinkingResolution();
};

#endif
