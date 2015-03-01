#include "hierarchyChecking.h"
#include "compilationUnit.h"
#include "classDecl.h"
#include "error.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "methodHeader.h"
#include "classMethod.h"
#include "interfaceMethod.h"
#include <iostream>
#include <queue>
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
            Token* token = cd->getClassId()->getToken();

            if (!cd->noSuperClass()) {
                Name* name = cd->getSuper()->getSuperName();
                CompilationTable* processing = retrieveCompilationOfTypeName(compilation, name, token);
                if (processing == NULL) { return; }

                if (processing->getSymbolTable() && !processing->isClassSymbolTable()) {
                    std::stringstream ss;
                    ss << "Class '" << processing->getClassOrInterfaceName() << "' cannot extend"
                       << " from an interface.";
                    Error(E_HIERARCHY, token, ss.str());
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
                std::set<std::string> method_signatures;
                std::pair<std::set<std::string>::iterator,bool> ret;

                while (cbd != NULL) {
                    if (cbd->isClassMethod()) {
                        MethodHeader* mh = static_cast<ClassMethod*>(cbd)->getMethodHeader();
                        std::string signature = mh->methodSignatureAsString();

                        ret = method_signatures.insert(signature);
                        if (ret.second == false) {
                            std::stringstream ss;
                            ss << "Class '" << compilation->getClassOrInterfaceName() << "' has multiple methods with the signature '"
                               << signature << "'.";

                            Error(E_HIERARCHY, token, ss.str());
                        }
                    }

                    cbd = cbd->getNextDeclaration();
                }
            }
        }
    } else if (st) {
        InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();
        token = id->getInterfaceId()->getToken();

        if (!id->emptyInterfaceBody()) {
            InterfaceBodyStar* ibs = id->getInterfaceBodyStar();

            if (!ibs->isEpsilon()) {
                InterfaceMethod* im = ibs->getInterfaceMethods();
                std::set<std::string> method_signatures;
                std::pair<std::set<std::string>::iterator,bool> ret;
                
                while (im != NULL) {
                    std::string signature = im->methodSignatureAsString();

                    ret = method_signatures.insert(signature);
                    if (ret.second == false) {
                        std::stringstream ss;
                        ss << "Interface '" << compilation->getClassOrInterfaceName() << "' has multiple methods with the signature '"
                           << signature << "'.";

                        Error(E_HIERARCHY, token, ss.str());
                    }
                    
                    im = im->getNextInterfaceMethod();
                }
            }
        }
    }
}

void HierarchyChecking::NoStaticOverride(CompilationTable* compilation) {
    std::set<CompilationTable*> visited;
    std::pair<std::set<CompilationTable*>::iterator,bool> visited_ret;
    std::queue<CompilationTable*> traverse;
    traverse.push(compilation);

    std::set<std::string> methods;
    std::pair<std::set<std::string>::iterator,bool> ret;
    CompilationTable* processing;
    while (!traverse.empty()) {
        processing = traverse.front();
        traverse.pop();

        visited_ret= visited.insert(processing);
        if (visited_ret.second == false || processing == NULL) {
            continue;
        }

        SymbolTable* st = processing->getSymbolTable();
        Token* token;

        if (processing->isClassSymbolTable()) {
            ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();
            token = cd->getClassId()->getToken();

            if (!cd->emptyBody()) {
                ClassBodyStar* cbs = cd->getClassMembers();

                if (!cbs->isEpsilon()) {
                    ClassBodyDecls* cbd = cbs->getBody();

                    while (cbd != NULL) {
                        if (cbd->isClassMethod()) {
                            MethodHeader* mh = static_cast<ClassMethod*>(cbd)->getMethodHeader();
                            std::string signature = mh->methodSignatureAsString();

                            ret = methods.insert(signature);
                            if (cbd->isStatic() && ret.second == false) {
                                std::stringstream ss;
                                ss << "Static method '" << signature << "' in class '" << processing->getClassOrInterfaceName()
                                   << "' cannot be overriden.";

                                Error(E_HIERARCHY, token, ss.str());
                                break;
                            }
                        }

                        cbd = cbd->getNextDeclaration();
                    }
                }
            }

            if (!cd->noSuperClass()) {
                Name* name = cd->getSuper()->getSuperName();

                processing = retrieveCompilationOfTypeName(compilation, name, token);
                if (processing == NULL) { break; }
                traverse.push(processing);
            }

            if (!cd->noImplementedInterfaces()) {
                Interfaces* il = cd->getImplementInterfaces()->getListOfInterfaces();

                while (il != NULL) {
                    traverse.push(il->getImplOrExtInterfaceTable());
                    il = il->getNextInterface();
                }
            }
        } else if (st) {
            InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();

            if (!id->emptyInterfaceBody()) {
                InterfaceBodyStar* ibs = id->getInterfaceBodyStar();
                Token* token = id->getInterfaceId()->getToken();

                if (!ibs->isEpsilon()) {
                    InterfaceMethod* im = ibs->getInterfaceMethods();

                    while (im != NULL) {
                        std::string signature = im->methodSignatureAsString();

                        ret = methods.insert(signature);
                        if (im->isStatic() && ret.second == false) {
                            std::stringstream ss;
                            ss << "Static method '" << signature << "' in interface '" << processing->getClassOrInterfaceName()
                               << "' cannot be overriden.";

                            Error(E_HIERARCHY, token, ss.str());
                        }

                        im = im->getNextInterfaceMethod();
                    }
                }
            }

            if (!id->noExtendedInterfaces()) {
                Interfaces* il = id->getExtendedInterfaces()->getListOfInterfaces();

                while (il != NULL) {
                    traverse.push(il->getImplOrExtInterfaceTable());
                    il = il->getNextInterface();
                }
            }
        }
    }
}

void HierarchyChecking::classNotImplementClass(CompilationTable* compilation, std::vector<CompilationTable*> currentPackage){
    TypeDecl *typedecl = compilation->getCompilationUnit()->getTypeDecl();
    if(typedecl->isClass() && !dynamic_cast<ClassDecl*>(typedecl)->noImplementedInterfaces())
    {
        Interfaces *interface = dynamic_cast<ClassDecl*>(typedecl)->getImplementInterfaces()->getListOfInterfaces();
        while(interface != NULL)
        {
            Name *interfaceName = interface->getCurrentInterface();
            std::vector<CompilationTable*> package;
            if(!interfaceName->isLastPrefix())
            {
                package = packages[interfaceName->getQualifier()];
            }
            else
            {
                package = currentPackage;
            }
            std::vector<CompilationTable*>::iterator it3;
            for (it3 = package.begin(); it3 != package.end(); it3++) 
            {
                std::cout << "interface name: " << interfaceName->getNameId()->getIdAsString() << std::endl;
                std::cout << "checking name: " << (*it3)->getClassOrInterfaceName() << std::endl;
                if((*it3)->getClassOrInterfaceName() == interfaceName->getNameId()->getIdAsString())
                {
                    if((*it3)->getCompilationUnit()->getTypeDecl()->isClass())
                    {
                        Error(E_HIERARCHY, interfaceName->getNameId()->getToken(), "error: class cannot implement a class\n");
                    }
                    else
                    {
                        break;
                    }
                }
            }
            assert(it3 != package.end());
            interface = interface->getNextInterface();
        }
    }
}

void HierarchyChecking::classNotExtendFinalClass(CompilationTable* compilation, std::vector<CompilationTable*> currentPackage){
    TypeDecl *typedecl = compilation->getCompilationUnit()->getTypeDecl();
    if(typedecl->isClass() && !dynamic_cast<ClassDecl*>(typedecl)->noSuperClass())
    {
        Name *superName = dynamic_cast<ClassDecl*>(typedecl)->getSuper()->getSuperName();
        std::vector<CompilationTable*> package;
        if(!superName->isLastPrefix())
        {
            package = packages[superName->getQualifier()];
        }
        else
        {
            package = currentPackage;
        }
        std::vector<CompilationTable*>::iterator it3;
        for (it3 = package.begin(); it3 != package.end(); it3++) 
        {
            if((*it3)->getClassOrInterfaceName() == superName->getNameId()->getIdAsString())
            {
                assert((*it3)->getCompilationUnit()->getTypeDecl()->isClass());
                Modifiers *modifiers = dynamic_cast<ClassDecl*>((*it3)->getCompilationUnit()->getTypeDecl())->getClassModifiers();
                while(modifiers != NULL)
                {
                    if(modifiers->getCurrentModifierAsString() == "final")
                    {
                        Error(E_HIERARCHY, superName->getNameId()->getToken(), "error: class cannot extend a final class\n");
                    }
                    modifiers = modifiers->getNextModifier();
                }
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
            
            classNotImplementClass(*it2, it->second);
            classNotExtendFinalClass(*it2, it->second);
            classNotExtendInterface(*it2);
            duplicateInterface(*it2);
            interfaceNotExtendClass(*it2);
            noDuplicateSignature(*it2);
            NoStaticOverride(*it2);

            if (Error::count() > 0) { return; }
        }
    }
}
