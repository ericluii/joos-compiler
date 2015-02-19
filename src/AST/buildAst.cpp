#include "buildAst.h"
// Class members
#include "fieldDecl.h"
#include "classMethod.h"
// Types
#include "primitiveType.h"
#include "referenceType.h"
// Primary
#include "literalOrThis.h"
#include "bracketedExpression.h"
#include "primaryNewArray.h"
// Access and Primary
#include "arrayAccessName.h"
#include "arrayAccessPrimary.h"
// Method invocation and Primary
#include "methodNormalInvoke.h"
#include "invokeAccessedMethod.h"
// Assignment
#include "assignName.h"
#include "assignArray.h"
#include "assignField.h"
// Expression
#include "binaryExpression.h"
#include "instanceof.h"
#include "primaryExpression.h"
#include "nameExpression.h"
#include "castName.h"
#include "castPrimitive.h"
#include "negationExpression.h"
// Block statements
#include "localDecl.h"
#include "ifStmt.h"
#include "emptyStmt.h"
#include "forStmt.h"
#include "nestedBlock.h"
#include "returnStmt.h"
#include "stmtExprAssign.h"
#include "stmtExprCreation.h"
#include "stmtExprInvoke.h"
#include "whileStmt.h"

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

    returnName = new Name(makeIdentifier(tree->children[0]->children[2]));
    returnName->setRule(tree->rule);
    Name* currentName = returnName;
    Name* qualifier;
    tree = tree->children[0]->children[0];
    
    while(true) {
        switch(tree->rule) {
            case NAME_QUALIFIED:
                qualifier = new Name(makeIdentifier(tree->children[0]->children[2]));
                qualifier->setRule(tree->rule);
                currentName->setNextName(qualifier);
                currentName = qualifier;
                tree = tree->children[0]->children[0];
                break;
            case NAME_SIMPLE:
                currentName->setNextName(makeName(tree));
                return returnName;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
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
        returnImport = new ImportDecls(makeName(tree->children[0]->children[0]->children[1]));
        returnImport->setRule(tree->rule);
        return returnImport;
    }
    
    assert(tree->rule == IMPORTS_LIST);
    
    returnImport = new ImportDecls(makeName(tree->children[1]->children[0]->children[1]));
    returnImport->setRule(tree->children[1]->rule);
    ImportDecls* currentImport = returnImport;
    ImportDecls* nextImport;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case IMPORTS_LIST:
                nextImport = new ImportDecls(makeName(tree->children[1]->children[0]->children[1]));
                nextImport->setRule(tree->children[1]->rule);
                currentImport->setNextImport(nextImport);
                currentImport = nextImport;
                tree = tree->children[0];
                break;
            case IMPORTS_DECL:
                currentImport->setNextImport(makeImportDecls(tree));
                return returnImport;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
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
        returnModifiers = new Modifiers(tree->children[0]->children[0]->token);
        returnModifiers->setRule(tree->rule);
        return returnModifiers;
    }

    assert(tree->rule == MODIFIERS_LIST || tree->rule == MEMBER_MOD_LIST);

    returnModifiers = new Modifiers(tree->children[1]->children[0]->token);
    returnModifiers->setRule(tree->rule);
    Modifiers* currentModifier = returnModifiers;
    Modifiers* nextModifier;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case MODIFIERS_LIST:
            case MEMBER_MOD_LIST:
                nextModifier = new Modifiers(tree->children[1]->children[0]->token);
                nextModifier->setNextModifier(nextModifier);
                currentModifier->setRule(tree->rule);
                currentModifier = nextModifier;
                break;
            case MODIFIERS_MOD:
            case MEMBER_MOD:
                currentModifier->setNextModifier(makeModifiers(tree));
                return returnModifiers;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
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
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
    return NULL;
}

InterfaceList* BuildAst::makeInterfaceList(ParseTree* tree) {
    if(debug) std::cout << "InterfaceList\n";
    InterfaceList* returnInterfaces = NULL;
    
    if(tree->rule == IMPLEMENT_EPSILON) {
        returnInterfaces = new InterfaceList(NULL);
        returnInterfaces->setRule(tree->rule);
        return returnInterfaces;
    }
    assert(tree->rule == IMPLEMENTING);
    returnInterfaces = new InterfaceList(makeImplements(tree->children[1]));
    returnInterfaces->setRule(tree->rule);
    return returnInterfaces;
}

Implements* BuildAst::makeImplements(ParseTree* tree) {
    if(debug) std::cout << "Implements\n";
    Implements* returnImplements = NULL;
    if(tree->rule == INTERFACE_TYPE_LIST_END) {
        returnImplements = new Implements(makeName(tree->children[0]->children[0]->children[0]));
        returnImplements->setRule(tree->rule);
        return returnImplements;
    }

    assert(tree->rule == INTERFACE_TYPE_LIST);
    returnImplements = new Implements(makeName(tree->children[2]->children[0]->children[0]));
    returnImplements->setRule(tree->rule);
    Implements* currentImplement = returnImplements;
    Implements* nextImplement;
    tree = tree->children[0];
    while(true) {
        switch(tree->rule) {
            case INTERFACE_TYPE_LIST:
                nextImplement = new Implements(makeName(tree->children[2]->children[0]->children[0]));
                nextImplement->setNextImplement(nextImplement);
                currentImplement = nextImplement;
                tree = tree->children[0];
                break;
            case INTERFACE_TYPE_LIST_END:
                currentImplement->setNextImplement(makeImplements(tree));
                return returnImplements;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
    return NULL;
}
    
ClassDecl *BuildAst::makeClassDecl(ParseTree *tree){
    //TODO actually make a ClassDecl
    if(debug) std::cout << "ClassDecl\n";
    assert(tree->rule == CLASS_DECL);

    return new ClassDecl(makeModifiers(tree->children[0]),
                         makeIdentifier(tree->children[2]),
                         makeSuper(tree->children[3]),
                         makeInterfaceList(tree->children[4]),
                         makeClassBodyStar(tree->children[5]->children[1]));
}

ClassBodyStar* BuildAst::makeClassBodyStar(ParseTree* tree) {
    if(debug) std::cout << "ClassBodyStar\n";
    if(tree->rule == CLASS_BODY_DECL_EPSILON) {
        return new ClassBodyStar(NULL);
    }

    assert(tree->rule == CLASS_BODY_DECLSTAR);
    return new ClassBodyStar(makeClassBodyDecls(tree->children[0]));
}

ClassBodyDecls* BuildAst::makeClassBodyDecls(ParseTree* tree) {
    ClassBodyDecls* returnClassBody;
    if(debug) std::cout << "ClassBodyDecls\n";
    if(tree->rule == CLASS_BODY_DECL) {
        return makeClassBodyDecl(tree->children[0]);
    }

    assert(tree->rule == CLASS_BODY_LIST);
    returnClassBody = makeClassBodyDecl(tree->children[1]);
    ClassBodyDecls* currentClassBody = returnClassBody;
    ClassBodyDecls* nextClassBody;
    tree = tree->children[0];
    
    while(true) {
        switch(tree->rule) {
            case CLASS_BODY_LIST:
                nextClassBody = makeClassBodyDecl(tree->children[1]);
                currentClassBody->setNextDeclaration(nextClassBody);
                currentClassBody = nextClassBody;
                tree = tree->children[0];
                break;
            case CLASS_BODY_DECL:
                currentClassBody->setNextDeclaration(makeClassBodyDecl(tree->children[0]));
                return returnClassBody;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false); 
        }
    }
    assert(false);
    return NULL;
}

ClassBodyDecls* BuildAst::makeClassBodyDecl(ParseTree* tree) {
    if(debug) std::cout << "ClassBodyDecl\n";
    
    if(tree->rule == CLASS_MEMBER) {
        return makeClassMember(tree->children[0]);
    }
    
    assert(tree->rule == CLASS_CONSTRUCTOR);
    std::cout << "Constructor\n";
    return new ClassBodyDecls(NULL);
}

ClassBodyDecls* BuildAst::makeClassMember(ParseTree* tree) {
    if(debug) std::cout << "ClassMember\n";
    ClassBodyDecls* returnClassMember = NULL;
    if(tree->rule == CLASS_FIELD) {
        Expression* expr = NULL;
        if(tree->children[0]->children[2]->rule == DECL_VARIABLE_ASSIGN) {
            expr = makeExpression(tree->children[0]->children[2]->children[2]);
        } else {
            assert(tree->children[0]->children[2]->rule == DECL_VARIABLE);
        }
        returnClassMember = new FieldDecl(makeModifiers(tree->children[0]->children[0]),
                                          makeType(tree->children[0]->children[1]),
                                          makeIdentifier(tree->children[0]->children[2]->children[0]),
                                          expr);
    } else {
        assert(tree->rule == CLASS_METHOD);
        tree = tree->children[0];
        returnClassMember = new ClassMethod(makeModifiers(tree->children[0]->children[0]),
                                            makeMethodHeader(tree->children[0]),
                                            makeMethodBody(tree->children[1]));
    }
    returnClassMember->setRule(tree->rule);
    return returnClassMember;
}

MethodHeader* BuildAst::makeMethodHeader(ParseTree* tree) {
    if(debug) std::cout << "MethodHeader\n";
    assert(tree->rule == METHOD_TYPE || tree->rule == METHOD_VOID);
    MethodHeader* header = NULL;
    Type* type = NULL;
    
    Identifier* id = makeIdentifier(tree->children[2]->children[0]);
    FormalParamStar* params = makeFormalParamStar(tree->children[2]->children[2]);
    if(tree->rule == METHOD_TYPE) {
        type = makeType(tree->children[1]);
    }

    header = new MethodHeader(id, type, params);
    header->setRule(tree->rule);
    return header;
}

FormalParamStar* BuildAst::makeFormalParamStar(ParseTree* tree) {
    if(debug) std::cout << "FormalParamStar\n";
    assert(tree->rule == FORMAL_PARAMSTAR || tree->rule == FORMAL_PARAMSTAR_EPSILON);
    FormalParamStar* paramStar = NULL;
    ParamList* paramList = NULL;
    
    if(tree->rule == FORMAL_PARAMSTAR) {
        paramList = makeParamList(tree->children[0]);
    }

    paramStar = new FormalParamStar(paramList);
    paramStar->setRule(tree->rule);
    return paramStar;
}

ParamList* BuildAst::makeParamList(ParseTree* tree) {
    if(debug) std::cout << "ParamList\n";
    ParamList* params = NULL;
    assert(tree->rule == FORMAL_PARAM || tree->rule == FORMAL_PARAM_LIST);

    if(tree->rule == FORMAL_PARAM) {
        params = new ParamList(makeType(tree->children[0]->children[0]),
                               makeIdentifier(tree->children[0]->children[1]));
        params->setRule(tree->rule);
        return params;
    }

    params = new ParamList(makeType(tree->children[1]->children[0]),
                           makeIdentifier(tree->children[1]->children[1]));
    params->setRule(tree->rule);
    ParamList* currentParams = params;
    ParamList* nextParams;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case FORMAL_PARAM_LIST:
                nextParams = new ParamList(makeType(tree->children[1]->children[0]),
                                           makeIdentifier(tree->children[1]->children[1]));
                nextParams->setRule(tree->rule);
                currentParams->setNextParameter(nextParams);
                currentParams = nextParams;
                tree = tree->children[0];
                break;
            case FORMAL_PARAM:
                currentParams->setNextParameter(makeParamList(tree));
                return params;
        }
    }
    assert(false);
    return NULL;
}

MethodBody* BuildAst::makeMethodBody(ParseTree* tree) {
    MethodBody* body = NULL;
    if(debug) std::cout << "MethodBody\n";
    assert(tree->rule == METHOD_BODY || tree->rule == METHOD_BODY_EMPTY);

    BlockStmtsStar* stmts = NULL;
    if(tree->rule == METHOD_BODY) {
        stmts = makeBlockStmtsStar(tree->children[0]->children[1]);
    }

    body = new MethodBody(stmts);
    body->setRule(tree->rule);
    return body;
}

BlockStmtsStar* BuildAst::makeBlockStmtsStar(ParseTree* tree) {
    BlockStmtsStar* returnStmts = NULL;
    if(debug) std::cout << "BlockStmtsStar\n";
    assert(tree->rule == BLOCK_STMTS || tree->rule == BLOCK_STMTS_EPSILON);

    BlockStmts* blockStmts = NULL;
    if(tree->rule == BLOCK_STMTS) {
        blockStmts = makeBlockStmts(tree->children[0]);
    }

    returnStmts = new BlockStmtsStar(blockStmts);
    returnStmts->setRule(tree->rule);
    return returnStmts;
}

BlockStmts* BuildAst::makeBlockStmts(ParseTree* tree) {
    BlockStmts* returnStmts = NULL;
    if(debug) std::cout << "BlockStmts\n";
    assert(tree->rule == BLOCK_STMT || tree->rule == BLOCK_STMT_LIST);

    if(tree->rule == BLOCK_STMT) {
        return makeSingleStmt(tree->children[0]);
    }

    returnStmts = makeSingleStmt(tree->children[1]);
    BlockStmts* currentStmt = returnStmts;
    BlockStmts* nextStmt;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case BLOCK_STMT_LIST:
                nextStmt = makeSingleStmt(tree->children[1]);
                currentStmt->setNextBlockStmt(nextStmt);
                currentStmt = nextStmt;
                tree = tree->children[0];
                break;
            case BLOCK_STMT:
                currentStmt->setNextBlockStmt(makeBlockStmts(tree));
                return returnStmts;
        }
    }
    assert(false);
    return returnStmts;
}

BlockStmts* BuildAst::makeSingleStmt(ParseTree* tree) {
    BlockStmts* singleStmt = NULL;
    if(debug) std::cout << "SingleStmt\n";
    assert(tree->rule == LOCAL_VAR_STMT || tree->rule == BLOCK_IS_STMT);

    if(tree->rule == LOCAL_VAR_STMT) {
        tree = tree->children[0]->children[0];
        singleStmt = new LocalDecl(makeType(tree->children[0]), makeIdentifier(tree->children[1]),
                                   makeExpression(tree->children[3]));
        singleStmt->setRule(LOCAL_VAR_STMT);
        return singleStmt;
    }

    singleStmt = makeStatement(tree->children[0]);
    return singleStmt;
}

BlockStmts* BuildAst::makeStatement(ParseTree* tree) {
    BlockStmts* singleStmt = NULL;
    if(debug) std::cout << "Statement\n";
    assert(tree->rule == STMT_NO_TRAILING || tree->rule == IF_STMT ||
           tree->rule == IF_THEN_STMT || tree->rule == WHILE_STMT || tree->rule == FOR_STMT ||
           tree->rule == NOSHORTIF_STMT_NON_TRAILING || tree->rule == NO_SHORT_IF_THEN ||
           tree->rule == NO_SHORT_WHILE || tree->rule == NO_SHORT_FOR);
    switch(tree->rule) {
        case IF_STMT:
        case IF_THEN_STMT:
        case NO_SHORT_IF_THEN:
            singleStmt = makeIfStmt(tree);
            break;
        case STMT_NO_TRAILING:
        case NOSHORTIF_STMT_NON_TRAILING:
            singleStmt = makeNoTrailingSubstatement(tree->children[0]);
            break;
        case WHILE_STMT:
        case NO_SHORT_WHILE:
            singleStmt = makeWhileStmt(tree);
            break;
        case FOR_STMT:
        case NO_SHORT_FOR:
            singleStmt = makeForStmt(tree);
            break;
    }

    return singleStmt;

}

BlockStmts* BuildAst::makeIfStmt(ParseTree* tree) {
    BlockStmts* ifStmt = NULL;
    if(debug) std::cout << "IfStmt\n";
    assert(tree->rule == IF_STMT || tree->rule == IF_THEN_STMT || tree->rule == NO_SHORT_IF_THEN);
    int rule = tree->rule;
    tree = tree->children[0];

    if(rule == IF_STMT) {
        ifStmt = new IfStmt(makeExpression(tree->children[2]), makeStatement(tree->children[4]), NULL);
    } else {
        ifStmt = new IfStmt(makeExpression(tree->children[2]), makeStatement(tree->children[4]),
                            makeStatement(tree->children[6]));
    }

    ifStmt->setRule(rule);
    return ifStmt;
}

BlockStmts* BuildAst::makeWhileStmt(ParseTree* tree) {
    BlockStmts* whileStmt = NULL;
    if(debug) std::cout << "WhileStmt\n";
    assert(tree->rule == WHILE_STMT || tree->rule == NO_SHORT_WHILE);

    int rule = tree->rule;
    tree = tree->children[0];
    whileStmt = new WhileStmt(makeExpression(tree->children[2]), makeStatement(tree->children[4]));
    whileStmt->setRule(rule);
    return whileStmt;
}

BlockStmts* BuildAst::makeForStmt(ParseTree* tree) {
    BlockStmts* forStmt = NULL;
    if(debug) std::cout << "ForStmt\n";
    assert(tree->rule == FOR_STMT || tree->rule == NO_SHORT_FOR);

    int rule = tree->rule;
    tree = tree->children[0];
    BlockStmts* forInit = NULL;
    ExpressionStar* expr = makeExpressionStar(tree->children[4]);
    StmtExpr* forUpdate = NULL;
    BlockStmts* loopStmt = makeStatement(tree->children[8]);

    if(tree->children[2]->rule == FOR_INIT_LOCAL_DECL) {
        ParseTree* backup = tree->children[2]->children[0];
        forInit = new LocalDecl(makeType(backup->children[0]), makeIdentifier(backup->children[1]),
                                makeExpression(backup->children[3]));
        forInit->setRule(tree->children[1]->rule);
    } else if(tree->children[2]->rule == FOR_INIT_STMT) {
        forInit = makeStmtExpr(tree->children[2]->children[0]);
        forInit->setRule(tree->children[2]->rule);
    }

    if(tree->children[6]->rule == FOR_UPDATE_STMT) {
        forUpdate = makeStmtExpr(tree->children[6]->children[0]);
        forUpdate->setRule(tree->children[6]->rule);
    }

    forStmt = new ForStmt(forInit, expr, forUpdate, loopStmt);
    forStmt->setRule(rule);
    return forStmt;
}

BlockStmts* BuildAst::makeNoTrailingSubstatement(ParseTree* tree) {
    BlockStmts* noTrailSubStmt = NULL;
    if(debug) std::cout << "NoTrailingSubstatement\n";
    assert(tree->rule == STMT_NON_TRAILING_BLOCK || tree->rule == STMT_NON_TRAILING_EMPTY ||
           tree->rule == STMT_NON_TRAILING_EXPR || tree->rule == STMT_NON_TRAILING_RETURN);

    int rule = tree->rule;
    if(rule == STMT_NON_TRAILING_BLOCK) {
        noTrailSubStmt = new NestedBlock(makeBlockStmtsStar(tree->children[0]->children[1]));
    } else if(rule == STMT_NON_TRAILING_EMPTY) {
        noTrailSubStmt = new EmptyStmt();
    } else if(rule == STMT_NON_TRAILING_EXPR) {
        noTrailSubStmt = makeStmtExpr(tree->children[0]->children[0]);
    } else {
        noTrailSubStmt = new ReturnStmt(makeExpressionStar(tree->children[0]->children[1]));
    }

    if(tree->rule != STMT_NON_TRAILING_EXPR) {
        noTrailSubStmt->setRule(rule);
    }
    return noTrailSubStmt;
}

StmtExpr* BuildAst::makeStmtExpr(ParseTree* tree) {
    StmtExpr* stmtExpr = NULL;
    if(debug) std::cout << "StmtExpr\n";
    assert(tree->rule == STMTEXPR_TO_ASSIGN || tree->rule == STMTEXPR_INVOKE ||
           tree->rule == STMTEXPR_MAKE_CLASS);

    if(tree->rule == STMTEXPR_TO_ASSIGN) {
        stmtExpr = new StmtExprAssign(makeAssignment(tree->children[0]));
    } else if(tree->rule == STMTEXPR_INVOKE) {
        stmtExpr = new StmtExprInvoke(makeMethodInvoke(tree->children[0]));
    } else {
        stmtExpr = new StmtExprCreation(makeClassCreation(tree->children[0]));
    }

    stmtExpr->setRule(tree->rule);
    return stmtExpr;
}

Type* BuildAst::makeType(ParseTree* tree) {
    if(debug) std::cout << "Type\n";
    Type* returnType = NULL;
    if(tree->rule == PRIMITIVE_TYPE) {
        returnType = new PrimitiveType(tree->children[0]->children[0]->token);
        returnType->setRule(tree->children[0]->rule);
        return returnType;
    }

    assert(tree->rule == REFERENCE_TYPE);
    returnType = makeReferenceType(tree->children[0]);

    return returnType;
}

Type* BuildAst::makeReferenceType(ParseTree* tree) {
    if(debug) std::cout << "ReferenceType\n";
    Type* returnType = NULL;
    switch(tree->rule) {
        case REFERENCE_CLASSINTERFACE:
            returnType = new ReferenceType(makeName(tree->children[0]->children[0]));
            returnType->setRule(tree->rule);
            break;
        case REFERENCE_ARRAY:
            if(tree->children[0]->rule == ARRAY_PRIMITIVE) {
                returnType = new PrimitiveType(tree->children[0]->children[0]->children[0]->token);
            } else {
                assert(tree->children[0]->rule == ARRAY_NONPRIMITIVE);
                returnType = new ReferenceType(makeName(tree->children[0]->children[0]));
            }

            returnType->setRule(tree->children[0]->rule);
            break;
        default:
            std::cerr << "None of the rules apply" << std::endl;
            assert(false);
    }
    return returnType;
}

ExpressionStar* BuildAst::makeExpressionStar(ParseTree* tree) {
    ExpressionStar* exprStar = NULL;
    if(debug) std::cout << "ExpressionStar\n";
    
    Expression* expr = NULL;
    if(tree->rule == EXPR_STAR) {
        expr = makeExpression(tree->children[0]);
    }

    exprStar = new ExpressionStar(expr);
    exprStar->setRule(tree->rule);
    return exprStar;
}

Expression* BuildAst::makeExpression(ParseTree* tree) {
    std::cout << "Expression\n";
    if(tree->rule == EXPRESSION_COND || tree->rule == ASSIGNEXPR_TO_COND) {
        std::cout << "ConditionalExpression\n";
        return makeBinaryExpression(tree->children[0]);
    }

    assert(tree->rule == EXPRESSION_ASSIGN || tree->rule == ASSIGNEXPR_TO_ASSIGN);
    return makeAssignment(tree->children[0]);
}

Expression* BuildAst::makeBinaryExpression(ParseTree* tree) {
    if(debug) std::cout << "BinaryExpression\n";
    Expression* retBinExpr = NULL;
    while(true) {
        switch(tree->rule) {
            case COND_TO_CONDOR:
            case CONDOR_TO_CONDAND:
            case CONDAND_TO_INCLUOR:
            case INCLUOR_TO_AND:
            case AND_TO_EQUALITY:
            case EQUALITY_TO_RELATIONAL:
            case RELATION_TO_ADDITIVE:
            case ADD_TO_MULTI:
                tree = tree->children[0];
                break;
            case MULTI_TO_UNARY:
                return makeUnaryExpression(tree->children[0]);
            case CONDOR_TO_CONDORAND:
            case CONDAND_TO_CONDANDINCLUOR:
            case INCLUOR_TO_INCLUORAND:
            case AND_TO_ANDEQUALITY:
            case EQUALITY_TO_EQUALITYRELATION:
            case EQUALITY_TO_NOTEQRELATION:
            case RELATION_TO_LTRELATIONADD:
            case RELATION_TO_GTRELATIONADD:
            case RELATION_TO_LTERELATIONADD:
            case RELATION_TO_GTERELATIONADD:
            case ADD_TO_PLUSMULTI:
            case ADD_TO_MINUSMULTI:
                retBinExpr = new BinaryExpression(makeBinaryExpression(tree->children[0]),
                                                  makeBinaryExpression(tree->children[2]));
                retBinExpr->setRule(tree->rule);
                return retBinExpr;
            case RELATION_TO_INSTANCEOF:
                retBinExpr = new InstanceOf(makeBinaryExpression(tree->children[0]),
                                            makeReferenceType(tree->children[2]));
                retBinExpr->setRule(tree->rule);
                return retBinExpr;
            case MULTI_TO_MULTUNARY:
            case MULTI_TO_DIVUNARY:
            case MULTI_TO_MODUNARY:
                retBinExpr = new BinaryExpression(makeBinaryExpression(tree->children[0]),
                                                  makeUnaryExpression(tree->children[2]));
                retBinExpr->setRule(tree->rule);
                return retBinExpr;
            default:
                std::cerr << "None of the rules applied" << std::endl;
                assert(false);
        }
    }
    return NULL;
}

Expression* BuildAst::makeUnaryExpression(ParseTree* tree) {
    if(debug) std::cout << "UnaryExpression\n";
    Expression* retUnaryExpr = NULL;
    assert(tree->rule == NEG_UNARY || tree->rule == NOT_NEG_UNARY);
    if(tree->rule == NEG_UNARY) {
        retUnaryExpr = new NegationExpression(makeUnaryExpression(tree->children[1]));
        retUnaryExpr->setRule(tree->rule);
    } else {
        retUnaryExpr = makeUnaryNotMinusExpr(tree->children[0]);
    }

    return retUnaryExpr;
}

Expression* BuildAst::makeUnaryNotMinusExpr(ParseTree* tree) {
    if(debug) std::cout << "UnaryNotMinusExpr\n";
    Expression* retUnaryExpr = NULL;
    assert(tree->rule == NOT_UNARY || tree->rule == UNARY_CAST || tree->rule == PRIMARY_UNARY
           || tree->rule == UNARY_NAME);
    if(tree->rule == NOT_UNARY) {
        retUnaryExpr = new NegationExpression(makeUnaryExpression(tree->children[1]));
    } else if(tree->rule == UNARY_CAST) {
        retUnaryExpr = makeCastExpression(tree->children[0]);
    } else if(tree->rule == PRIMARY_UNARY) {
        retUnaryExpr = new PrimaryExpression(makePrimary(tree->children[0]));
    } else {
        retUnaryExpr = new NameExpression(makeName(tree->children[0]));
    }

    if(tree->rule == UNARY_CAST) {
        retUnaryExpr->setRule(tree->rule);
    }
    return retUnaryExpr;
}

Expression* BuildAst::makeCastExpression(ParseTree* tree) {
    if(debug) std::cout << "CastExpression\n";
    Expression* retCastExpr = NULL;
    assert(tree->rule == CAST_PRIMITIVE || tree->rule == CAST_NONPRIMITIVE ||
           tree->rule == CAST_TO_EXPRESSION);
    if(tree->rule == CAST_PRIMITIVE) {
        PrimitiveType* type = new PrimitiveType(tree->children[1]->children[0]->token);
        type->setRule(tree->children[1]->rule);
        retCastExpr = new CastPrimitive(tree->children[2]->rule, type, makeUnaryExpression(tree->children[4]));
    } else if(tree->rule == CAST_NONPRIMITIVE) {
        retCastExpr = new CastName(makeName(tree->children[1]), makeUnaryNotMinusExpr(tree->children[5]));
    } else {
        Expression* expr = makeUnaryNotMinusExpr(tree->children[3]);
        tree = tree->children[1];
        while(true) {
            switch(tree->rule) {
                case COND_TO_CONDOR:
                case CONDOR_TO_CONDORAND:
                case CONDAND_TO_INCLUOR:
                case INCLUOR_TO_AND:
                case AND_TO_EQUALITY:
                case EQUALITY_TO_RELATIONAL:
                case RELATION_TO_ADDITIVE:
                case ADD_TO_MULTI:
                case MULTI_TO_UNARY:
                case NOT_NEG_UNARY:
                    tree = tree->children[0];
                    break;
                case UNARY_NAME:
                    retCastExpr = new CastName(makeName(tree->children[0]), expr);
                    retCastExpr->setRule(tree->rule);
                    return retCastExpr;
            }
        }
    }

    retCastExpr->setRule(tree->rule);
    return retCastExpr;
}

Assignment* BuildAst::makeAssignment(ParseTree* tree) {
    Assignment* returnAssign = NULL;
    if(debug) std::cout << "Assignment\n";
    int rule = tree->children[0]->rule;
    assert(rule == LVALUE_NAME || rule == LVALUE_FIELD_ACCESS || rule == LVALUE_ARRAY);
    Expression* expr = makeExpression(tree->children[2]);
    tree = tree->children[0];
    if(rule == LVALUE_NAME) {
        returnAssign = new AssignName(makeName(tree->children[0]), expr);
    } else if(rule == LVALUE_FIELD_ACCESS) {
        returnAssign = new AssignField(makeFieldAccess(tree->children[0]), expr);
    } else {
        returnAssign = new AssignArray(makeArrayAccess(tree->children[0]), expr);
    }

    returnAssign->setRule(rule);
    return returnAssign;
}

FieldAccess* BuildAst::makeFieldAccess(ParseTree* tree) {
    if(debug) std::cout << "FieldAccess\n";
    assert(tree->rule == FIELD_ACCESS);
    FieldAccess* returnFieldAccess = new FieldAccess(makeIdentifier(tree->children[2]));
    returnFieldAccess->setRule(tree->rule);
    FieldAccess* currentFieldAccess = returnFieldAccess;
    FieldAccess* newFieldAccess;
    tree = tree->children[0];
    
    while(true) {
        switch(tree->rule) {
            case PRIMARY_FIELDACCESS:
                newFieldAccess = new FieldAccess(makeIdentifier(tree->children[0]->children[2]));
                newFieldAccess->setRule(FIELD_ACCESS);
                currentFieldAccess->setPrimaryField(newFieldAccess);
                currentFieldAccess = newFieldAccess;
                tree = tree->children[0];
                break;
            default:
                currentFieldAccess->setPrimaryField(makePrimary(tree));
                return returnFieldAccess;
        }
    }
    assert(false);
    return NULL;
}

ArrayAccess* BuildAst::makeArrayAccess(ParseTree* tree) {
    if(debug) std::cout << "ArrayAccess\n";
    assert(tree->rule == ACCESS_ARRAY_NAME || tree->rule == ACCESS_ARRAY_PRIMARY);
    Expression* expr = makeExpression(tree->children[2]);
    ArrayAccess* returnAAN;
    if(tree->rule == ACCESS_ARRAY_NAME) {
        returnAAN = new ArrayAccessName(makeName(tree->children[0]), expr);
    } else {
        returnAAN = new ArrayAccessPrimary(makePrimaryNonArray(tree->children[0]), expr);
    }

    returnAAN->setRule(tree->rule);
    return returnAAN;
}

Primary* BuildAst::makePrimary(ParseTree* tree) {
    if(debug) std::cout << "Primary\n";
    if(tree->rule == PRIMARY_NONARRAY) {
        return makePrimaryNonArray(tree->children[0]);
    }

    assert(tree->rule == PRIMARY_NEWARRAY);
    return makePrimaryNewArray(tree->children[0]);
}

Primary* BuildAst::makePrimaryNonArray(ParseTree* tree) {
    Primary* primaryNA = NULL;
    if(debug) std::cout << "PrimaryNonArray\n";
    int rule = tree->rule;
    assert(rule == PRIMARY_LITERAL || rule == PRIMARY_THIS || rule == PRIMARY_EXPRESSION || rule == PRIMARY_MAKECLASS
           || rule == PRIMARY_FIELDACCESS || rule == PRIMARY_INVOKE || rule == PRIMARY_ARRAY_ACCESS);

    if(rule == PRIMARY_LITERAL) {
        primaryNA = new LiteralOrThis(tree->children[0]->children[0]->token);
    } else if(rule == PRIMARY_THIS) {
        primaryNA = new LiteralOrThis(tree->children[0]->token);
    } else if(rule == PRIMARY_EXPRESSION) {
        primaryNA = new BracketedExpression(makeExpression(tree->children[1]));
    } else if(rule == PRIMARY_MAKECLASS) {
        primaryNA = makeClassCreation(tree->children[0]);
    } else if(rule == PRIMARY_FIELDACCESS) {
        primaryNA = makeFieldAccess(tree->children[0]);
    } else if(rule == PRIMARY_INVOKE) {
        primaryNA = makeMethodInvoke(tree->children[0]);
    } else {
        primaryNA = makeArrayAccess(tree->children[0]);
    }

    if(rule == PRIMARY_LITERAL) {
        primaryNA->setRule(tree->children[0]->rule);
    } else if(rule != PRIMARY_FIELDACCESS && rule != PRIMARY_ARRAY_ACCESS
              && rule != PRIMARY_MAKECLASS) {
        primaryNA->setRule(tree->rule);
    }
    
    return primaryNA;
}

NewClassCreation* BuildAst::makeClassCreation(ParseTree* tree) {
    if(debug) std::cout << "ClassCreation\n";
    assert(tree->rule == MAKE_NEW_CLASS);
    Name* classType = makeName(tree->children[1]->children[0]->children[0]);
    ArgumentsStar* args = makeArgumentsStar(tree->children[3]);
    NewClassCreation* returnNCC = new NewClassCreation(classType, args);
    returnNCC->setRule(tree->rule);
    return returnNCC;
}

ArgumentsStar* BuildAst::makeArgumentsStar(ParseTree* tree) {
    if(debug) std::cout << "ArgumentsStar\n"; 
    ArgumentsStar* returnArgsStar;
    if(tree->rule == ARG_LIST) {
        returnArgsStar = new ArgumentsStar(makeArguments(tree->children[0]));
        returnArgsStar->setRule(tree->rule);
        return returnArgsStar;
    }

    assert(tree->rule == ARG_LIST_EPSILON);
    returnArgsStar = new ArgumentsStar(NULL);
    returnArgsStar->setRule(tree->rule);
    return returnArgsStar;
}

Arguments* BuildAst::makeArguments(ParseTree* tree) {
    if(debug) std::cout << "Arguments\n";
    Arguments* returnArguments;
   
    if(tree->rule == ARG_LIST_EXPRESSION) {
        returnArguments = new Arguments(makeExpression(tree->children[0]));
        returnArguments->setRule(ARG_LIST_EXPRESSION);
        return returnArguments;
    }
    
    assert(tree->rule == ARG_LIST_LIST);

    returnArguments = new Arguments(makeExpression(tree->children[1]));
    returnArguments->setRule(tree->rule);
    Arguments* currentArgs = returnArguments;
    Arguments* nextArgs;
    tree = tree->children[0];

    while(true) {
        switch(tree->rule) {
            case ARG_LIST_LIST:
                nextArgs = new Arguments(makeExpression(tree->children[1]));
                nextArgs->setRule(tree->rule);
                currentArgs->setNextArgs(nextArgs);
                currentArgs = nextArgs;
                tree = tree->children[0];
                break;
            case ARG_LIST_EXPRESSION:
                currentArgs->setNextArgs(makeArguments(tree));
                return returnArguments;
            default:
                std::cerr << "None of the rules apply" << std::endl;
                assert(false);
        }
    }
    assert(false);
    return NULL;
}

MethodInvoke* BuildAst::makeMethodInvoke(ParseTree* tree) {
    if(debug) std::cout << "MethodInvoke\n";
    MethodInvoke* invocation = NULL;
    assert(tree->rule == INVOKE_METHOD_NORM || tree->rule == INVOKE_METHOD_ACCESS);
    ArgumentsStar* args = makeArgumentsStar(tree->children[2]);
    if(tree->rule == INVOKE_METHOD_NORM) {
        invocation = new MethodNormalInvoke(makeName(tree->children[0]), args);
    } else {
        invocation = new InvokeAccessedMethod(makeFieldAccess(tree->children[0]), args);
    }

    invocation->setRule(tree->rule);
    return invocation;
}

Primary* BuildAst::makePrimaryNewArray(ParseTree* tree) {
    Primary* returnPNA = NULL;
    if(debug) std::cout << "PrimaryNewArray\n";
    assert(tree->rule == MAKE_NEW_PRIMITIVE_ARRAY || tree->rule == MAKE_NEW_NONPRIMITIVE_ARRAY);
    Expression* expr = makeExpression(tree->children[2]->children[1]);
    if(tree->rule == MAKE_NEW_PRIMITIVE_ARRAY) {
        returnPNA = new PrimaryNewArray(makeType(tree->children[1]), expr);
    } else {
        if(debug) std::cout << "Type\n";
        Type* type = new ReferenceType(makeName(tree->children[1]->children[0]));
        type->setRule(REFERENCE_CLASSINTERFACE);
        returnPNA = new PrimaryNewArray(type, expr);
    }

    returnPNA->setRule(tree->rule);
    return returnPNA;
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
