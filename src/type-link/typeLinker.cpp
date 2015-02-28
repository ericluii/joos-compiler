#include <sstream>
#include <iostream>
#include "typeLinker.h"
#include "compilationUnit.h"
#include "error.h"

TypeLinker::TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

TypeLinker::~TypeLinker() {}

void TypeLinker::checkSingleImportClashError(const std::string& typeName, ImportDecls* imports) {
    std::string importType;
    Token* importTok;
    if(imports->isSingleImport()) {
        importTok = imports->getCurrentImport()->getNameId()->getToken();
        importType = importTok->getString();
        if(importType == typeName) {
            std::stringstream ss;
            ss << "Single type import '" << imports->getCurrentImport()->getFullName()
               << "' clashes with type '" << typeName << "'.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    }
}

void TypeLinker::checkImportsClashWithType(const std::string& typeName, ImportDecls* imports) {
    checkSingleImportClashError(typeName, imports);

    while(!imports->isLastImport()) {
        imports = imports->getNextImport();
        checkSingleImportClashError(typeName, imports);
    }
}

void TypeLinker::checkForClashingSingleImportInFile(CompilationTable* compilation) {
    std::string classOrInterfaceName = compilation->getClassOrInterfaceName();
    if(classOrInterfaceName == "") {
        // indicates that no type was defined
        return;
    }
    CompilationUnit* unit = compilation->getCompilationUnit();
    ImportDeclsStar* imports = unit->getImportDeclsStar();
    if(!imports->isEpsilon()) {
        checkImportsClashWithType(classOrInterfaceName, imports->getImportDeclarations());
    }
}

void TypeLinker::setImportIfExist(CompilationTable* compilation, ImportDecls* import) {
    std::stringstream ss;
    Name* importName = import->getCurrentImport();
    Token* importTok = importName->getNameId()->getToken();
    std::string packageName  = importName->getQualifier();
    std::string typeName = importTok->getString();
  
    if(import->isSingleImport()) {
        if(packageName != "") {
            if(packages.count(packageName) == 1) {
                bool typeExist = false;
                for(unsigned int i = 0; i < packages[packageName].size(); i++) {
                    if(packages[packageName][i]->getClassOrInterfaceName() == typeName) {
                        compilation->setASingleTypeImport(typeName, packages[packageName][i], importTok);
                        typeExist = true;
                    }
                }

                if(!typeExist) {
                    // the type wasn't found in the package
                    ss << "Single type import '" << packageName << "." << typeName
                       << "' specified an undefined type.";
                    Error(E_TYPELINKING, importTok, ss.str());
                }
            } else {
                // package does not exist
                ss << "Single type import specified package '" << packageName
                   << "' which does not exist among the given files in the command line.";
                Error(E_TYPELINKING, importName->getNextName()->getNameId()->getToken(), ss.str());
            }
        } else {
            // package named is default package, which is not possible
            ss << "Single type import '" << typeName << "' didn't specify a package.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    } else {
        // import on demand
        std::string fullPackageName;
        if(packageName != "") {
            fullPackageName = packageName + '.';
        }
        fullPackageName+= typeName;

        if(packages.count(fullPackageName) == 1) {
            compilation->setAnImportTypeOnDemand(fullPackageName, &packages[fullPackageName]);
        } else {
            bool prefixPackage = false;
            // this is to avoid prefixes like jav in java.lang, which would be invalid
            std::string prefixPackageName = fullPackageName + '.';
            std::map<std::string, std::vector<CompilationTable*> >::iterator it;
            for(it = packages.begin(); it != packages.end(); it++) {
                if(it->first != "") {
                    // if not the default package
                    if(it->first.find(prefixPackageName) == 0) {
                        // if the package name is actually a prefix of some package name
                        // the package won't really have any compilation table associated with it
                        compilation->setAnImportTypeOnDemand(fullPackageName, NULL); 
                        prefixPackage = true;
                    }
                }
            }

            if(!prefixPackage) {
                // if the package name is not a prefix of any package name
                // specified in the given files
                ss << "Import type on demand '" << fullPackageName
                   << ".*' did not specify an existing package or a prefix of some existing package.";
                Error(E_TYPELINKING, importTok, ss.str());
            }
        }
    }
}

void TypeLinker::importsExistenceRecurse(CompilationTable* compilation, ImportDecls* import) {
    if(!import->isLastImport()) {
        importsExistenceRecurse(compilation, import->getNextImport());
    }

    setImportIfExist(compilation, import);
}

void TypeLinker::checkImportsExistenceAndSet(CompilationTable* compilation) {
    ImportDeclsStar* imports = compilation->getCompilationUnit()->getImportDeclsStar();
    if(!imports->isEpsilon()) {
        importsExistenceRecurse(compilation, imports->getImportDeclarations());
    }
}

void TypeLinker::typeLinkingResolution() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            checkForClashingSingleImportInFile(*it2);
            CHECK_ERROR();
            checkImportsExistenceAndSet(*it2);
            CHECK_ERROR();
        }
    }
}
