#include "CompilationUnit.h"

CompilationUnit::CompilationUnit(PackageDecl *package, ImportDecl *import, ClassDecl *classDecl)
{
    this->packageDecl = package;
    this->importDecl = import;
    this->classDecl = classDecl;
    this->interfaceDecl = NULL;
}

CompilationUnit::CompilationUnit(PackageDecl *package, ImportDecl *import, InterfaceDecl *interface)
{
    this->packageDecl = package;
    this->importDecl = import;
    this->classDecl = NULL;
    this->interfaceDecl = interface;
}

CompilationUnit::CompilationUnit(PackageDecl *package, ImportDecl *import)
{
    this->packageDecl = package;
    this->importDecl = import;
    this->classDecl = NULL;
    this->interfaceDecl = NULL;
}

CompilationUnit::PackageDecl *GetPackageDecl(){
    return this->packageDecl;
}

CompilationUnit::ImportDecl *GetImportDecl(){
    return this->importDecl;
}

CompilationUnit::ClassDecl *GetClassDecl(){
    return this->classDecl;
}

CompilationUnit::InterfaceDecl *GetInterfaceDecl(){
    return this->interfaceDecl;
}