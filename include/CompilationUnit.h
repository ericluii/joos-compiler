#ifndef __COMPILATIONUNIT_H__
#define __COMPILATIONUNIT_H__

class CompilationUnit
{
        PackageDecl *packageDecl;
        ImportDecl *importDecl;
        ClassDecl *classDecl;
        InterfaceDecl *interfaceDecl;
    public:
        CompilationUnit(PackageDecl *package, ImportDecl *import, ClassDecl *classDecl);       
        CompilationUnit(PackageDecl *package, ImportDecl *import, InterfaceDecl *interface);
        CompilationUnit(PackageDecl *package, ImportDecl *import);
        
        PackageDecl *GetPackageDecl();
        ImportDecl *GetImportDecl();
        ClassDecl *GetClassDecl();
        InterfaceDecl *GetInterfaceDecl();
}

#endif
