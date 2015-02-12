#ifndef __AST_H__
#define __AST_H__

#include "ruleNumbers.h"
#include "parseTree.h"
#include <iostream>

#include "classDecl.h"
#include "compilationUnit.h"
#include "importDecls.h"
#include "importDeclsStar.h"
#include "identifier.h"
#include "interfaceDecl.h"
#include "name.h"
#include "onDemandImportDecls.h"
#include "packageDecl.h"
#include "singleTypeImportDecls.h"
#include "typeDecl.h"

class ast
{
        static const bool debug = true;
    public:
        ast(){}
        static CompilationUnit *makeCompilationUnit(ParseTree *tree);
        static PackageDecl *makePackageDecl(ParseTree *tree);
        static ImportDeclsStar *makeImportDeclsStar(ParseTree *tree);
        static ImportDecls *makeImportDecls(ParseTree *tree);
        static TypeDecl *makeTypeDecl(ParseTree *tree);
        static ClassDecl *makeClassDecl(ParseTree *tree);
        static InterfaceDecl *makeInterfaceDecl(ParseTree *tree);
        static Name *makeName(ParseTree *tree);
        static Identifier *makeIdentifier(ParseTree *tree);
        
};


#endif
