#include ast.h
#include <cassert>

ast::ast(){}
    
CompilationUnit *ast::makeCompilationUnit(ParseTree *tree){
    return new CompilationUnit(makePackageDecl(tree->children[0]),
                               makeImportDeclsStar(tree->children[1]),
                               makeTypeDecl(tree->children[2]));
}

PackageDecl *ast::makePackageDecl(ParseTree *tree){
    if(tree->rule == PACKAGE_NAME){
        return makeName(tree->children[0]);
    }
    assert(tree->rule == PACKAGE_EPSILON)
    return new PackageDecl();
}

ImportDeclsStar *ast::makeImportDeclsStar(ParseTree *tree){
    if(tree->rule == IMPORT_STAR_DECLS){
        //TODO call makeImportDecls once it is written
        return new ImportDeclsStar();
    }
    assert(tree->rule == IMPORT_STAR_EPSILON)
    return new ImportDeclsStar();
}

TypeDecl *ast::makeTypeDecl(ParseTree *tree){
    if(tree->rule == TYPE_CLASS){
        return makeClassDecl(tree->children[0]);
    }
    else if(tree->rule == TYPE_INTERFACE){
        return makeInterfaceDecl(tree->children[0]);
    }
    assert(tree->rule == TYPE_EPSILON)
    return new TypeDecl();
}

ClassDecl *ast::makeClassDecl(ParseTree *tree){
    //TODO actually make a ClassDecl
    return new ClassDecl();
}

InterfaceDecl *ast::makeInterfaceDecl(ParseTree *tree){
    //TODO actually make a InterfaceDecl
    return new InterfaceDecl();
}

Name *ast::makeName(ParseTree *tree){
    //TODO actually make a Name
    return new Name();
}
