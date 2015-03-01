#ifndef __TYPELINKER_H__
#define __TYPELINKER_H__

#include <map>
#include <vector>
#include <string>
#include "compilationTable.h"

class PackageDecl;
class ImportDecls;
class CompilationUnit;
class ClassDecl;
class InterfaceDecl;
class Super;

class TypeLinker {
    private:
        std::map<std::string, std::vector<CompilationTable*> >& packages;
        // Related to single type imports clashing with types in the same file
        void checkSingleImportClashError(const std::string& typeName, const std::string& packageName, ImportDecls* imports);
        void checkImportsClashWithType(const std::string& typeName, const std::string& packageName, ImportDecls* imports);
        void checkForClashingSingleImportInFile(CompilationTable*);

        // Related to checking for imported types or packages to exist
        void setImportIfExist(CompilationTable* compilation, ImportDecls* import);
        void importsExistenceRecurse(CompilationTable* compilation, ImportDecls* import);
        void checkImportsExistenceAndSet(CompilationTable* compilation);

        // Related to whether a name conflicts with a type
        bool checkIfNameConflictsWithType(CompilationTable* compilation, Name* name);

        // Related to checking if packages or prefixes of packages/imports resolve to types
        void checkImportsResolveToTypes(CompilationTable* compilation, ImportDecls* import);
        void checkPackageResolveToTypes(CompilationTable* compilation, PackageDecl* package);
        void checkPackageAndImportsResolveToTypes(CompilationTable* compilation);

        // Related to making sure all names that resolve to types refer to some class or interface
        void linkTypeNames(CompilationTable* compilation, InterfaceDecl* type);
        CompilationTable* linkTypeNames(CompilationTable* compilation, Name* name);
        void linkTypeNames(CompilationTable* compilaiton, Super* super);
        void linkTypeNames(CompilationTable* compilation, ClassDecl* type);
        void linkTypeNames(CompilationTable* compilation, CompilationUnit* unit);
    public:
        TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages);
        ~TypeLinker();

        void typeLinkingResolution();
};

#endif
