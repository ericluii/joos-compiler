#include "buildAst.h"
#include <cassert>
    
CompilationUnit *BuildAst::makeCompilationUnit(ParseTree *tree){
    if(debug) std::cout << "compilationunit\n";
    assert(tree->rule == COMPILATION_UNIT);
    CompilationUnit* returnCompUnit = new CompilationUnit(makePackageDecl(tree->children[0]),
                                                          makeImportDeclsStar(tree->children[1]),
                                                          makeTypeDecl(tree->children[2]));
    returnCompUnit->setRule(tree->rule);
    return returnCompUnit;
}

Identifier *BuildAst::makeIdentifier(ParseTree *tree){
    if(debug) std::cout << "Identifier\n";
    assert(tree->rule == IDENTIFIER);
    Identifier* returnIdentifier = new Identifier(tree->children[0]->token);
    returnIdentifier->setRule(tree->rule);
    return returnIdentifier;
}

Name *BuildAst::makeName(ParseTree *tree){
    Name* returnName;
    if(debug) std::cout << "Name\n";
    if(tree->rule == NAME_SIMPLE){
        returnName = new Name(makeIdentifier(tree->children[0]->children[0]));
        returnName->setRule(tree->rule);
        return returnName;
    }
    assert(tree->rule == NAME_QUALIFIED);

    tree = tree->children[0];
    returnName = new Name(makeIdentifier(tree->children[2]));
    Name* currentName = returnName;
    Name* qualifier;
    tree = tree->children[0];
    
    while(true) {
        switch(tree->rule) {
            case NAME_QUALIFIED:
                qualifier = new Name(makeIdentifier(tree->children[2]));
                currentName->setNextName(qualifier);
                currentName->setRule(tree->rule);
                currentName = qualifier;
                tree = tree->children[0];
                break;
            case NAME_SIMPLE:
                currentName->setNextName(makeName(tree));
                currentName->setRule(tree->rule);
                return returnName;
        }
    }
    return NULL;
}

PackageDecl *BuildAst::makePackageDecl(ParseTree *tree){
    if(debug) std::cout << "packageDecl\n";
    PackageDecl* package;
    if(tree->rule == PACKAGE_NAME) {
        package = new PackageDecl(makeName(tree->children[1]));
    }
    assert(tree->rule == PACKAGE_EPSILON);
    package = new PackageDecl(NULL);
    package->setRule(tree->rule);
    return package;
}

ImportDeclsStar *BuildAst::makeImportDeclsStar(ParseTree *tree){
    if(debug) std::cout << "ImportDeclsStar\n";
    ImportDeclsStar* returnImportStar;
    if(tree->rule == IMPORT_STAR_DECLS){
        returnImportStar = new ImportDeclsStar(makeImportDecls(tree->children[0]));
        returnImportStar->setRule(tree->rule);
        return returnImportStar;
    }
    assert(tree->rule == IMPORT_STAR_EPSILON);
    returnImportStar = new ImportDeclsStar(NULL);
    returnImportStar->setRule(tree->rule);
    return returnImportStar;
}

ImportDecls *BuildAst::makeImportDecls(ParseTree *tree){
    if(debug) std::cout << "ImportDecls\n";
    ImportDecls* returnImport;
    if(tree->rule == IMPORTS_DECL){
        tree = tree->children[0];
        assert(tree->rule == IMPORT_ON_DEMAND || tree->rule == SINGLE_TYPE_IMPORT);
        returnImport = new ImportDecl(makeName(tree->children[0]));
        returnImport->setRule(tree->rule);
        return returnImport;
    }
    
    assert(tree->rule == IMPORTS_LIST);
    
    returnImport = new ImportDecls(makeName(tree->children[1]->children[0]));
    ImportDecls* currentImport = returnImport;
    ImportDecls* nextImport;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case IMPORTS_LIST:
                nextImport = new ImportDecls(makeName(tree->children[1]));
                currentImport->setNextImport(nextImport);
                currentImport->setRule(tree->rule);
                currentImport = nextImport;
                tree = tree->children[0];
                break;
            case IMPORTS_DECL:
                currentImport->setNextImport(makeImportDecls(tree));
                currentImport->setRule(tree->rule);
                return returnImport;
        }
    }
    return NULL;
}

TypeDecl *BuildAst::makeTypeDecl(ParseTree *tree) {
    TypeDecl* returnType;
    if(debug) std::cout << "TypeDecl\n";
    if(tree->rule == TYPE_CLASS){
        returnType = makeClassDecl(tree->children[0]);
    }
    else if(tree->rule == TYPE_INTERFACE){
        returnType = makeInterfaceDecl(tree->children[0]);
    } 
    else { 
        assert(tree->rule == TYPE_EPSILON);
        returnType = new TypeDecl();
    }
    returnType->setRule(tree->rule);

    return returnType;
}

Modifiers* BuildAst::makeModifiers(ParseTree* tree) {
    if(debug) std::cout << "Modifiers\n";
    Modifiers* returnModifiers;

    if(tree->rule == MODIFIERS_MOD || tree->rule == MEMBER_MOD) {
        returnModifiers = new Modifiers(tree->children[0]->token);
        returnModifiers->setRule(tree->rule);
        return returnModifiers;
    }

    assert(tree->rule == MODIFIERS_LIST || tree->rule == MEMBER_MOD_LIST);

    returnModifiers = new Modifiers(tree->children[1]->children[0]->token);
    Modifiers* currentModifier = returnModifiers;
    Modifiers* nextModifier;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case MODIFIERS_LIST:
            case MEMBER_MOD_LIST:
                nextModifier = new Modifiers(tree->children[1]->children[0]->token);
                currentModifier->setNextModifier(nextModifier);
                currentModifier->setRule(tree->rule);
                currentModifier = nextModifier;
                break;
            case MODIFIERS_MOD:
            case MEMBER_MOD:
                currentModifier->setNextModifier(new Modifiers(tree->children[1]->children[0]->token));
                currentModifier->setRule(tree->rule);
                return returnModifiers;
        }
    }
    return NULL;
}

Super* BuildAst::makeSuper(ParseTree* tree) {
    if(debug) std::cout << "Super\n";
    Super* returnSuper;
    
    if(tree->rule == EXTENDS_CLASS_EPSILON) {
        returnSuper = new Super(NULL);
        returnSuper->setRule(tree->rule);
        return returnSuper;
    }
    assert(tree->rule == EXTENDS_CLASS);

    while(true) {
        switch(tree->rule) {
            case EXTENDS_CLASS:
                tree = tree->children[1];
                break;
            case CLASS_TYPE:
                tree = tree->children[0];
                break;
            case CLASS_OR_INTERFACE_NAME:
                returnSuper = new Super(makeName(tree->children[0]));
                returnSuper->setRule(tree->rule);
                return returnSuper;
        }
    }

    return NULL;
}

Implements* BuildAst::makeImplements(ParseTree* tree) {
    if(debug) std::cout << "Implements\n";
    Implements* returnImplements;

    if(tree->rule == IMPLEMENT_EPSILON) {
        returnImplements = new Implements(NULL);
        returnImplements->setRule(tree->rule);
        return returnImplements;
    }
    assert(tree->rule == IMPLEMENTING);

    tree = tree->children[1];

    if(tree->rule == INTERFACE_TYPE_LIST_END) {
        returnImplements = new Implements(makeName(tree->children[0]->children[0]->children[0]));
        returnImplements->setRule(tree->rule);
    }

    assert(tree->rule == INTERFACE_TYPE_LIST);
    Name* currentImplement = makeName(tree->children[1]->children[0]->children[0]);
    Name* nextImplement;
    tree = tree->children[0];
    while(true) {
        switch(tree->rule) {
            case INTERFACE_TYPE_LIST:
                nextImplement = makeName(tree->children[1]->children[0]->children[0]);
                currentImplement->setNextName(nextImplement);
                currentImplement = nextImplement;
                break;
            case INTERFACE_TYPE_LIST_END:
                currentImplement->setNextName(makeName(tree->children[0]->children[0]->children[0]));
                returnImplements = new Implements(currentImplement);
                returnImplements->setRule(tree->rule);
                return returnImplements;
        }
    }

    return NULL;
}
    
ClassDecl *BuildAst::makeClassDecl(ParseTree *tree){
    //TODO actually make a ClassDecl
    if(debug) std::cout << "ClassDecl\n";
    assert(tree->rule == CLASS_DECL);

    return new ClassDecl(makeModifiers(tree->children[0]),
                         makeIdentifier(tree->children[2]),
                         makeSuper(tree->children[3]),
                         makeImplements(tree->children[4]),
                         makeClassBodyStar(tree->children[5]));
}

ClassBodyStar* BuildAst::makeClassBodyStar(ParseTree* tree) {
    return NULL;
}
ClassBodyDecls* BuildAst::makeClassBodyDecls(ParseTree* tree) {
    return NULL;
}

InterfaceDecl *BuildAst::makeInterfaceDecl(ParseTree *tree){
    //TODO actually make a InterfaceDecl
    if(debug) std::cout << "InterfaceDecl\n";
    assert(tree->rule == INTERFACE_DECL);
    return new InterfaceDecl();
}

Ast* BuildAst::build(ParseTree* tree) {
    return makeCompilationUnit(tree);
}
