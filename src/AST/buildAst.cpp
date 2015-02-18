#include "buildAst.h"
// Class members
#include "fieldDecl.h"
// Types
#include "primitiveType.h"
#include "referenceType.h"
// Primary
#include "literalOrThis.h"
#include "primaryExpr.h"
#include "primaryNewArray.h"
#include "newClassCreation.h"
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
        returnClassMember = new ClassBodyDecls(NULL);
    }
    returnClassMember->setRule(tree->rule);
    return returnClassMember;
}

Type* BuildAst::makeType(ParseTree* tree) {
    if(debug) std::cout << "Type\n";
    Type* returnType = NULL;
    if(tree->rule == PRIMITIVE_TYPE) {
        returnType = new PrimitiveType(tree->children[0]->children[0]->token);
        returnType->setRule(tree->rule);
        return returnType;
    }

    assert(tree->rule == REFERENCE_TYPE);
    tree = tree->children[0];

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

Expression* BuildAst::makeExpression(ParseTree* tree) {
    std::cout << "Expression\n";
    if(tree->rule == EXPRESSION_COND || tree->rule == ASSIGNEXPR_TO_COND) {
        std::cout << "ConditionalExpression\n";
        return new Expression();
    }

    assert(tree->rule == EXPRESSION_ASSIGN || tree->rule == ASSIGNEXPR_TO_ASSIGN);
    return makeAssignment(tree->children[0]);
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
                return currentFieldAccess;
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
        primaryNA = new PrimaryExpr(makeExpression(tree->children[1]));
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

Primary* BuildAst::makeClassCreation(ParseTree* tree) {
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
