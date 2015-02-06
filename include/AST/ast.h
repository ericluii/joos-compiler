#ifndef __AST_H__
#define __AST_H__

#include ruleNumbers.h
#include compilationUnit.h

class ast
{
    public:
        ast();
    
        CompilationUnit *makeCompilationUnit(ParseTree *tree);
        PackageDecl *makePackageDecl(ParseTree *tree);
        ImportDeclsStar *makeImportDeclsStar(ParseTree *tree);
        TypeDecl *makeTypeDecl(ParseTree *tree);
        ClassDecl *makeClassDecl(ParseTree *tree);
        InterfaceDecl *makeInterfaceDecl(ParseTree *tree);
        Name *makeName(ParseTree *tree);
        
}


#endif