#include "ast.h"
#include <cassert>
    
CompilationUnit *ast::makeCompilationUnit(ParseTree *tree){
    if(debug) std::cout << "compilationunit\n";
    return new CompilationUnit(makePackageDecl(tree->children[0]),
                               makeImportDeclsStar(tree->children[1]),
                               makeTypeDecl(tree->children[2]));
}

PackageDecl *ast::makePackageDecl(ParseTree *tree){
    if(debug) std::cout << "packageDecl\n";
    if(tree->rule == PACKAGE_NAME){
        return makeName(tree->children[0]);
    }
    assert(tree->rule == PACKAGE_EPSILON);
    return new PackageDecl();
}

ImportDeclsStar *ast::makeImportDeclsStar(ParseTree *tree){
    if(debug) std::cout << "ImportDeclsStar\n";
    if(tree->rule == IMPORT_STAR_DECLS){
        return makeImportDecls(tree->children[0]);
    }
    assert(tree->rule == IMPORT_STAR_EPSILON);
    return new ImportDeclsStar();
}

ImportDecls *ast::makeImportDecls(ParseTree *tree){
    if(debug) std::cout << "ImportDecls\n";
    if(tree->rule == IMPORTS_DECL){
        tree = tree->children[0];
        if(tree->rule == SINGLE_TYPE_IMPORT){
            return new SingleTypeImportDecl(makeName(tree->children[1]), NULL);
        }
        assert(tree->rule == IMPORT_ON_DEMAND);
        return new OnDemandImportDecl(makeName(tree->children[1]), NULL);
    }
    
    assert(tree->rule == IMPORTS_LIST);
    
    ParseTree *import = tree->children[1];
    if(import->rule == SINGLE_TYPE_IMPORT){
        return new SingleTypeImportDecl(makeName(import->children[1]), 
                                        makeImportDecls(tree->children[0]));
    }
    assert(import->rule == IMPORT_ON_DEMAND);
    return new OnDemandImportDecl(makeName(import->children[1]), 
                                  makeImportDecls(tree->children[0]));
}

TypeDecl *ast::makeTypeDecl(ParseTree *tree){
    if(debug) std::cout << "TypeDecl\n";
    if(tree->rule == TYPE_CLASS){
        return makeClassDecl(tree->children[0]);
    }
    else if(tree->rule == TYPE_INTERFACE){
        return makeInterfaceDecl(tree->children[0]);
    }
    assert(tree->rule == TYPE_EPSILON);
    return new TypeDecl();
}

ClassDecl *ast::makeClassDecl(ParseTree *tree){
    //TODO actually make a ClassDecl
    if(debug) std::cout << "ClassDecl\n";
    return new ClassDecl();
}

InterfaceDecl *ast::makeInterfaceDecl(ParseTree *tree){
    //TODO actually make a InterfaceDecl
    if(debug) std::cout << "InterfaceDecl\n";
    return new InterfaceDecl();
}

Name *ast::makeName(ParseTree *tree){
    if(debug) std::cout << "Name\n";
    if(tree->rule == NAME_SIMPLE){
        return new Name(makeIdentifier(tree->children[0]->children[0]));
    }
    assert(tree->rule == NAME_QUALIFIED);
    return new Name(makeName(tree->children[0]->children[0]),
                    makeIdentifier(tree->children[0]->children[2]));
}

Identifier *ast::makeIdentifier(ParseTree *tree){
    if(debug) std::cout << "Identifier\n";
    return new Identifier(tree->token);
}
