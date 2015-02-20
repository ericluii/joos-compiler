#ifndef __COMPILATIONUNIT_H__
#define __COMPILATIONUNIT_H__

#include "ast.h"
#include "packageDecl.h"
#include "importDeclsStar.h"
#include "typeDecl.h"

class CompilationUnit : public Ast
{
    // Rule: COMPILATION_UNIT
    private:
        PackageDecl *packageDecl;
        ImportDeclsStar *importDecls;
        TypeDecl *typeDecl;
    public:      
        CompilationUnit(PackageDecl *package, ImportDeclsStar *import,
                        TypeDecl *typeDecl) : packageDecl(package), importDecls(import), typeDecl(typeDecl) {}
        ~CompilationUnit() {
            delete packageDecl;
            delete importDecls;
            delete typeDecl;
        }

        PackageDecl *GetPackageDecl() { return packageDecl; }
        ImportDeclsStar *GetImportDeclsStar() { return importDecls; }
        TypeDecl *GetTypeDecl() { return typeDecl; }

        bool noTypeDeclared() { return typeDecl->isEpsilon(); }
        bool noPackageNamed() { return packageDecl->isEpsilon(); }
        bool noImports() { return importDecls->isEpsilon(); }
};

#endif
