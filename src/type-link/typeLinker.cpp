#include <sstream>
#include <iostream>
#include "typeLinker.h"
#include "compilationUnit.h"
#include "error.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "super.h"

TypeLinker::TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

TypeLinker::~TypeLinker() {}

void TypeLinker::checkSingleImportClashError(const std::string& typeName, const std::string& packageName,
                                             ImportDecls* imports) {
    std::string importType;
    std::string importQualifier;
    Token* importTok;
    if(imports->isSingleImport()) {
        importTok = imports->getCurrentImport()->getNameId()->getToken();
        importQualifier = imports->getCurrentImport()->getQualifier();
        importType = importTok->getString();
        if(importType == typeName && importQualifier != packageName) {
            // if the import type clashes with the type defined in this file
            // and it's not an import of the type in this file
            std::stringstream ss;
            ss << "Single type import '" << imports->getCurrentImport()->getFullName()
               << "' clashes with type '" << typeName << "'.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    }
}

void TypeLinker::checkImportsClashWithType(const std::string& typeName, const std::string& packageName,
                                           ImportDecls* imports) {
    checkSingleImportClashError(typeName, packageName, imports);

    while(!imports->isLastImport()) {
        imports = imports->getNextImport();
        checkSingleImportClashError(typeName, packageName, imports);
    }
}

void TypeLinker::checkForClashingSingleImportInFile(CompilationTable* compilation) {
    std::string classOrInterfaceName = compilation->getClassOrInterfaceName();
    if(classOrInterfaceName != "") {
        // indicates that a type was defined
        CompilationUnit* unit = compilation->getCompilationUnit();
        ImportDeclsStar* imports = unit->getImportDeclsStar();
        if(!imports->isEpsilon()) {
            checkImportsClashWithType(classOrInterfaceName, compilation->getPackageName(), imports->getImportDeclarations());
        }
    }
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

bool TypeLinker::checkIfNameConflictsWithType(CompilationTable* compilation, Name* name) {
    if(name == NULL) {
        return false;
    }
    
    std::string qualifier = name->getQualifier();
    if(qualifier != "") {
        Token* potentialTypeTok = name->getNameId()->getToken();
        std::string potentialTypeName = potentialTypeTok->getString();

        // if not the default package
        if(packages.count(qualifier) == 1) {
            std::vector<CompilationTable*>::iterator it;
            for(it = packages[qualifier].begin(); it != packages[qualifier].end(); it++) {
                if((*it)->getClassOrInterfaceName() == potentialTypeName) {
                    // indicates that the package name is actually a type name
                    return true;
                }
            }
        }
    }

    return checkIfNameConflictsWithType(compilation, name->getNextName());
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------


void TypeLinker::checkImportsResolveToTypes(CompilationTable* compilation, ImportDecls* import) {
    if(!import->isLastImport()) {
        checkImportsResolveToTypes(compilation, import->getNextImport());
    }
   
    std::stringstream ss;
    if(import->isSingleImport()) {
        if(checkIfNameConflictsWithType(compilation, import->getCurrentImport()->getNextName())) {
            ss << "Single type import '" << import->getCurrentImport()->getFullName()
               << "' or a prefix of it resolves to a type.";
            Error(E_TYPELINKING, import->getCurrentImport()->getNameId()->getToken(), ss.str());
        }
    } else {
        // import type on demand
        if(checkIfNameConflictsWithType(compilation, import->getCurrentImport())) {
            ss << "Import on demand '" << import->getCurrentImport()->getFullName()
               << "' or a prefix of it resolves to a type.";
            Error(E_TYPELINKING, import->getCurrentImport()->getNameId()->getToken(), ss.str());
        }
    }
}

void TypeLinker::checkPackageResolveToTypes(CompilationTable* compilation, PackageDecl* package) {
    if(checkIfNameConflictsWithType(compilation, package->getPackageName())) {
        std::stringstream ss;
        ss << "Package name '" << compilation->getPackageName() << "' refers to a type.";
        Error(E_TYPELINKING, package->getPackageName()->getNameId()->getToken(), ss.str());
    }
}

void TypeLinker::checkPackageAndImportsResolveToTypes(CompilationTable* compilation) {
    CompilationUnit* unit = compilation->getCompilationUnit();
    PackageDecl* package = unit->getPackageDecl();
    ImportDeclsStar* imports = unit->getImportDeclsStar();

    checkPackageResolveToTypes(compilation, package);
    if(!imports->isEpsilon()) {
        checkImportsResolveToTypes(compilation, imports->getImportDeclarations());
    }
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

void TypeLinker::linkTypeNames(CompilationTable* compilation, InterfaceDecl* type) {}

CompilationTable* TypeLinker::linkTypeNames(CompilationTable* compilation, Name* name) {
    std::string typeName = name->getNameId()->getIdAsString();
    CompilationTable* linkType = NULL;
    if(name->isQualifiedName()) {
        std::string qualifier = name->getQualifier();
        if(packages.count(qualifier) == 1) {
            bool typeExist = false;
            unsigned int indexExist = 0;
            for(unsigned int i = 0; i != packages[qualifier].size(); i++) {
                if(packages[qualifier][i]->getClassOrInterfaceName() == typeName) {
                    indexExist = i;
                    typeExist = true;
                    break;
                }
            }

            if(!typeExist) { return NULL; }
            checkIfNameConflictsWithType(compilation, name);
            linkType = packages[qualifier][indexExist];
        }
    } else {
        // simple name
        linkType = compilation->checkTypePresenceFromSingleImport(typeName);
        if(linkType != NULL) { return linkType; }
        linkType = compilation->checkTypePresenceInPackage(typeName);
        if(linkType != NULL) { return linkType; }
        linkType = compilation->checkTypePresenceFromImportOnDemand(typeName);
    }

    return linkType;
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Super* super) {
    if(!super->isEpsilon()) {
        CompilationTable* linkType = linkTypeNames(compilation, super->getSuperName());
        if(linkType == NULL) {
            std::stringstream ss;
            ss << "Extending class '" << super->getSuperName()->getFullName() 
               << "' is not possible because it does not exist.";
            Error(E_TYPELINKING, super->getSuperName()->getNameId()->getToken(), ss.str());
        }
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ClassDecl* type) {
    linkTypeNames(compilation, type->getSuper());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, CompilationUnit* unit) {
    if(unit->getTypeDecl()->isClass()) {
        linkTypeNames(compilation, (ClassDecl*) unit->getTypeDecl());
    } else {
        linkTypeNames(compilation, (InterfaceDecl*) unit->getTypeDecl());
    }
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------


void TypeLinker::typeLinkingResolution() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            checkForClashingSingleImportInFile(*it2);
            if (Error::count() > 0) { return; }
            checkImportsExistenceAndSet(*it2);
            if (Error::count() > 0) { return; }
            checkPackageAndImportsResolveToTypes(*it2);
            if (Error::count() > 0) { return; }
            linkTypeNames(*it2, (*it2)->getCompilationUnit());
            if (Error::count() > 0) { return; }
        }
    }
}