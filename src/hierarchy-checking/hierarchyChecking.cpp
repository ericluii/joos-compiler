#include "hierarchyChecking.h"
#include "compilationUnit.h"
#include "error.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include <iostream>
#include <sstream>
#include <set>
#include <cassert>

HierarchyChecking::HierarchyChecking(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

CompilationTable* HierarchyChecking::retrieveCompilationOfTypeName(CompilationTable* compilation, Name* name, Token* token) {
    CompilationTable* next_compilation = NULL;
    std::string typeName = name->getNameId()->getIdAsString();

    if (name->isQualifiedName()) {
        std::string qualifier = name->getQualifier();
        for (unsigned int i = 0; i < packages[qualifier].size(); i++) {
            if (packages[qualifier][i]->getClassOrInterfaceName() == typeName) {
                return packages[qualifier][i];
            }
        }
    } else {
        next_compilation = compilation->checkTypePresenceFromSingleImport(typeName);
        if (next_compilation != NULL) { return next_compilation; }

        next_compilation = compilation->checkTypePresenceInPackage(typeName);
        if (next_compilation != NULL) { return next_compilation; }

        next_compilation = compilation->checkTypePresenceFromImportOnDemand(typeName);
        if (next_compilation != NULL) { return next_compilation; }

        // Check java.lang if not found
        std::vector<CompilationTable*>::iterator it2;
        for (it2 = packages["java.lang"].begin(); it2 != packages["java.lang"].end(); it2++) {
            if ((*it2)->getClassOrInterfaceName() == typeName) {
                return (*it2);
            }
        }
    }

    std::stringstream ss;
    if (compilation->isClassSymbolTable()) {
        ss << "Class '" << compilation->getClassOrInterfaceName() << "' must include the qualified name"
           << " for '" << typeName << "' or import the package.";
    } else {
       ss << "Interface '" << compilation->getClassOrInterfaceName() << "' must include the qualified name"
          << " for '" << typeName << "' or import the package.";
    }
    Error(E_HIERARCHY, token, ss.str());

    return NULL;
}

void HierarchyChecking::classNotExtendInterface(CompilationTable* compilation) {
    if (compilation->isClassSymbolTable()) {
        SymbolTable* st = compilation->getSymbolTable();

        if (st->isClassTable()) {
            ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();

            if (!cd->noSuperClass()) {
                Name* name = cd->getSuper()->getSuperName();
                std::string packageName = name->getQualifier();

                for (unsigned int i = 0; i < packages[packageName].size(); i++) {
                    if (packages[packageName][i]->getClassOrInterfaceName() == name->getNameId()->getIdAsString()) {
                        if (packages[packageName][i]->getSymbolTable() && !packages[packageName][i]->isClassSymbolTable()) {
                            std::stringstream ss;
                            ss << "Class '" << packages[packageName][i]->getClassOrInterfaceName() << "' cannot extend"
                               << " from an interface.";
                            Error(E_HIERARCHY, cd->getClassId()->getToken(), ss.str());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void HierarchyChecking::duplicateInterface(CompilationTable* compilation) {
    InterfaceList* interface_list = NULL;
    SymbolTable* st = compilation->getSymbolTable();
    Token* token;

    if (compilation->isClassSymbolTable()) {
        ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();
        token = cd->getClassId()->getToken();

        if (!cd->noImplementedInterfaces()) {
            interface_list = cd->getImplementInterfaces();
        }
    } else if (st) {
        InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();
        token = id->getInterfaceId()->getToken();

        if (!id->noExtendedInterfaces()) {
            interface_list = id->getExtendedInterfaces();
        }
    }

    if (interface_list != NULL) {
        Interfaces* interface = interface_list->getListOfInterfaces();

        std::set<std::string> extendsOrImplements;
        std::pair<std::set<std::string>::iterator,bool> ret;
        while (interface != NULL) {
            Name* name = interface->getCurrentInterface();

            CompilationTable* source = retrieveCompilationOfTypeName(compilation, name, token);
            // Error generated in retrieveCompilationOfTypeName.
            if (source == NULL) { break; }

            ret = extendsOrImplements.insert(source->getCanonicalName());
            if (ret.second == false) {
                std::stringstream ss;
                if (compilation->isClassSymbolTable()) {
                    ss << "Class '" << compilation->getClassOrInterfaceName() << "' cannot implement "
                       << name->getNameId()->getIdAsString() << " multiple times.";
                } else {
                    ss << "Interface '" << compilation->getClassOrInterfaceName() << "' cannot extend "
                       << name->getNameId()->getIdAsString() << " multiple times.";
                }

                Error(E_HIERARCHY, token, ss.str());
                break; 
            }

            interface = interface->getNextInterface();
        }
    }
}

void HierarchyChecking::interfaceNotExtendClass(CompilationTable* compilation) {
    SymbolTable* st = compilation->getSymbolTable();
    Token* token;

    if (st && !compilation->isClassSymbolTable()) {
        InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();
        token = id->getInterfaceId()->getToken();

        if (!id->noExtendedInterfaces()) {
            Interfaces* interface = id->getExtendedInterfaces()->getListOfInterfaces();

            while (interface != NULL) {
                Name* name = interface->getCurrentInterface();

                CompilationTable* source = retrieveCompilationOfTypeName(compilation, name, token);
                // Error generated in retrieveCompilationOfTypeName.
                if (source == NULL) { break; }

                if (source->isClassSymbolTable()) {
                    std::stringstream ss;
                    ss << "Interface '" << compilation->getClassOrInterfaceName() << "' cannot extend "
                       << "from a class.";

                    Error(E_HIERARCHY, token, ss.str());
                    break;
                }

                interface = interface->getNextInterface();
            }
        }
    }
}

void HierarchyChecking::noDuplicateSignature(CompilationTable* compilation) {
    SymbolTable* st = compilation->getSymbolTable();
    Token* token;

    if (compilation->isClassSymbolTable()) {
        ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();
        token = cd->getClassId()->getToken();

        if (!cd->emptyBody()) {
            ClassBodyStar* cbs = cd->getClassMembers();

            if (!cbs->isEpsilon()) {
                ClassBodyDecls* cbd = cbs->getBody();
            }
        }
    } else if (st) {
        InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();
        token = id->getInterfaceId()->getToken();
    }
}

void HierarchyChecking::check() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for (it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            // PLACE CHECKS HERE
            classNotExtendInterface(*it2);
            duplicateInterface(*it2);
            interfaceNotExtendClass(*it2);
            noDuplicateSignature(*it2);

            if (Error::count() > 0) { return; }
        }
    }
}
