#ifndef __COMPILATIONUNIT_H__
#define __COMPILATIONUNIT_H__

#include "packageDecl.h"
#include "importDeclsStar.h"
#include "typeDecl.h"

class CompilationUnit
{
        PackageDecl *packageDecl;
        ImportDeclsStar *importDecls;
        TypeDecl *typeDecl;
    public:      
        CompilationUnit(PackageDecl *package, ImportDeclsStar *import, TypeDecl *typeDecl);
        
        PackageDecl *GetPackageDecl();
        ImportDeclsStar *GetImportDeclsStar();
        TypeDecl *GetTypeDecl();
};

#endif
