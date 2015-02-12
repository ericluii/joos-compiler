#include "compilationUnit.h"

CompilationUnit::CompilationUnit(PackageDecl *package, ImportDeclsStar *import, TypeDecl *typeDecl)
{
    this->packageDecl = package;
    this->importDecls = import;
    this->typeDecl = typeDecl;
}

PackageDecl *CompilationUnit::GetPackageDecl(){
    return this->packageDecl;
}

ImportDeclsStar *CompilationUnit::GetImportDeclsStar(){
    return this->importDecls;
}

TypeDecl *CompilationUnit::GetTypeDecl(){
    return this->typeDecl;
}