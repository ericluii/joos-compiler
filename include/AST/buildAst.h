#ifndef __BUILDAST_H__
#define __BUILDAST_H__

#include "ruleNumbers.h"
#include "parseTree.h"
#include <iostream>

#include "ast.h"
#include "classDecl.h"
#include "compilationUnit.h"
#include "importDecls.h"
#include "importDeclsStar.h"
#include "import.h"
#include "identifier.h"
#include "interfaceDecl.h"
#include "name.h"
#include "packageDecl.h"
#include "typeDecl.h"
#include "modifiers.h"
#include "super.h"
#include "implements.h"

class BuildAst
{
    private:
        static const bool debug = true;
        static CompilationUnit *makeCompilationUnit(ParseTree *tree);
        static PackageDecl *makePackageDecl(ParseTree *tree);
        static ImportDeclsStar *makeImportDeclsStar(ParseTree *tree);
        static ImportDecls *makeImportDecls(ParseTree *tree);
        static TypeDecl *makeTypeDecl(ParseTree *tree);
        static ClassDecl *makeClassDecl(ParseTree *tree);
        static InterfaceDecl *makeInterfaceDecl(ParseTree *tree);
        static Name *makeName(ParseTree *tree);
        static Identifier *makeIdentifier(ParseTree *tree);
        static Modifiers* makeModifiers(ParseTree* tree);
        static Super* makeSuper(ParseTree* tree);
        static Implements* makeImplements(ParseTree* tree);
        static ClassBodyStar* makeClassBodyStar(ParseTree* tree);
        static ClassBodyDecls* makeClassBodyDecls(ParseTree* tree);
    public:
        static Ast* build(ParseTree* tree);
};


#endif
