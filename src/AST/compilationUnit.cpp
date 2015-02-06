#include "compilationUnit.h"

CompilationUnit::CompilationUnit(PackageDecl *package, ImportDeclsStar *import, TypeDecl *typeDecl)
{
    this->packageDecl = package;
    this->importDecls = import;
    this->typeDecl = typeDecl;
}

CompilationUnit::PackageDecl *GetPackageDecl(){
    return this->packageDecl;
}

CompilationUnit::ImportDeclsStar *GetImportDeclsStar(){
    return this->importDecls;
}

CompilationUnit::TypeDecl *GetTypeDecl(){
    return this->typeDecl;
}