// AST
#include "classDecl.h"
#include "classBodyStar.h"
#include "classBodyDecls.h"
#include "fieldDecl.h"
#include "classMethod.h"
#include "constructor.h"
#include "expression.h"
#include "binaryExpression.h"
#include "arrayAccessName.h"
#include "arrayAccessPrimary.h"
#include "primaryExpression.h"
#include "name.h"
#include "bracketedExpression.h"
#include "fieldAccess.h"
#include "methodNormalInvoke.h"
#include "invokeAccessedMethod.h"
#include "newClassCreation.h"
#include "primaryNewArray.h"

// Symbol table
#include "classTable.h"

// Code generation related
#include "codeGenerator.h"
#include "startup.h"
#include "vtableManager.h"
#include "compilationTable.h"
#include "vtableLayout.h"
#include "objectLayout.h"

CodeGenerator::CodeGenerator(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) :
            compilations(compilations), firstUnit(firstUnit), starter(new Startup(compilations, firstUnit)),
            virtualManager(new VTableManager(compilations)), fs(NULL) {}

CodeGenerator::~CodeGenerator() {
    delete starter;
    delete virtualManager;

    std::map<CompilationTable*, ObjectLayout*>::iterator it;
    for(it = layoutOfClasses.begin(); it != layoutOfClasses.end(); it++) {
        delete it->second;
    }
}

void CodeGenerator::initStage() {
    // keep this order
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        starter->createTablesForCompilation(it->second);
        virtualManager->createVTableLayoutForCompilation(it->second);
        createObjectLayoutForCompilation(it->second);
    }

    starter->createTablesForArrayType();
    virtualManager->createVTableLayoutForArrays();
    // generate _startup.s
    starter->generateStartupFile(virtualManager->getVTableLayoutOfType(".array"));

    // starter->printInheritanceTable();
    // starter->printInterfaceMethodTable();
}

void CodeGenerator::createObjectLayoutForCompilation(CompilationTable* table) {
    if(table->aTypeWasDefined() && table->isClassSymbolTable()) {
        // a type was defined and it's a class
        if(layoutOfClasses.count(table) == 1) {
            // layout of class for this compilation has been made
            // already, return silently
            return;
        }

        ClassDecl* aClass = ((ClassTable*) table->getSymbolTable())->getClass();
        CompilationTable* superclass = aClass->getSuper()->getSuperClassTable();
        ObjectLayout* superclassLayout = NULL;
        if(superclass != NULL) {
            // create the object layout for the superclass first
            createObjectLayoutForCompilation(superclass);
            superclassLayout = layoutOfClasses[superclass];
        }
        layoutOfClasses[table] = new ObjectLayout(superclassLayout, table);
    }
}

// --------------------------------------------------------------------------------
// Code generation section

void CodeGenerator::traverseAndGenerate() {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            // a type was defined and it's a class
            fs = new std::ofstream(it->second->getCanonicalName() + ".s");
            traverseAndGenerate(((ClassTable*)it->second->getSymbolTable())->getClass());
            delete fs;
            fs = NULL;
        }
    }
}

void CodeGenerator::traverseAndGenerate(ClassDecl* aClass) {
    traverseAndGenerate(aClass->getClassMembers());
}

void CodeGenerator::traverseAndGenerate(ClassBodyStar* body) {
    if(!body->isEpsilon()) {
        traverseAndGenerate(body->getBody());
    }
}

void CodeGenerator::traverseAndGenerate(ClassBodyDecls* body) {
    if(!body->isLastClassMember()) {
        traverseAndGenerate(body->getNextDeclaration());
    }

    if(body->isField()) {
        traverseAndGenerate((FieldDecl*) body);
    } else if(body->isClassMethod()) {
        traverseAndGenerate((ClassMethod*) body);
    } else {
        traverseAndGenerate((Constructor*) body);
    }
}

void CodeGenerator::traverseAndGenerate(FieldDecl* field) {
    if(field->isInitialized()) {
        traverseAndGenerate(field->getInitializingExpression());
    }
}

void CodeGenerator::traverseAndGenerate(ClassMethod* method) {
}

void CodeGenerator::traverseAndGenerate(Constructor* ctor) {
}

void CodeGenerator::traverseAndGenerate(Expression* expr) {
    if(expr->isRegularBinaryExpression()) {
        traverseAndGenerate((BinaryExpression*) expr);
    } else if(expr->isPrimaryExpression()) {
    } else if(expr->isNameExpression()) {
    } else if(expr->isNumericNegation() || expr->isBooleanNegation()) {
    } else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
    } else if(expr->isInstanceOf()) {
    } else {
        // assignment
    }
}

void CodeGenerator::traverseAndGenerate(BinaryExpression* binExpr) {
    traverseAndGenerate(binExpr->getLeftExpression());

    if(binExpr->isAddition()) {
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isLazyOr()) {
        // both operands are evaluated first, except for || and &&
    } else if(binExpr->isLazyAnd()) {
    } else if(binExpr->isEagerOr()) {
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isEagerAnd()) {
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isEqual()) {
         traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isNotEqual()) {
         traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isLT()) {
          traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isLTE()) {
         traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isGT()) {
         traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isGTE()) {
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isAddition()) {
         traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isMinus()) {
          traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isMultiplication()) {
           traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isDivision()) {
            traverseAndGenerate(binExpr->getRightExpression());
    } else {
        // modulo
        traverseAndGenerate(binExpr->getRightExpression());
    }
}

void CodeGenerator::traverseAndGenerate(PrimaryExpression* primExpr) {
    traverseAndGenerate(primExpr->getPrimaryExpression());
}

void CodeGenerator::traverseAndGenerate(Primary* prim) {
    if(prim->isArrayAccessName() || prim->isArrayAccessPrimary()) {
        traverseAndGenerate((ArrayAccess*) prim);
    } else if(prim->isBracketedExpression()) {
        traverseAndGenerate(((BracketedExpression*) prim)->getExpressionInside());
    } else if(prim->isFieldAccess()) {
        traverseAndGenerate((FieldAccess*) prim);
    } else if(prim->isNormalMethodCall() || prim->isAccessedMethodCall()) {
        traverseAndGenerate((MethodInvoke*) prim);
    } else if(prim->isNewClassCreation()) {
        traverseAndGenerate((NewClassCreation*) prim);
    } else if(prim->isNewPrimitiveArray() || prim->isNewReferenceArray()) {
    } else if(prim->isQualifiedThis()) {
    } else {
        // LiteralOrThis
    }
}

void CodeGenerator::traverseAndGenerate(ArrayAccess* access) {
    traverseAndGenerate(access->getAccessExpression());
    if(access->isArrayAccessName()) {
        traverseAndGenerate(((ArrayAccessName*) access)->getNameOfAccessedArray());
    } else {
        traverseAndGenerate(((ArrayAccessPrimary*) access)->getAccessedPrimaryArray());
    }
}

void CodeGenerator::traverseAndGenerate(Name* name) {
    if(!name->isLastPrefix()) {
        traverseAndGenerate(name->getNextName());
    }
}

void CodeGenerator::traverseAndGenerate(FieldAccess* access) {
    traverseAndGenerate(access->getAccessedFieldPrimary());
}

void CodeGenerator::traverseAndGenerate(MethodInvoke* invoke) {
    traverseAndGenerate(invoke->getArgsForInvokedMethod());
    if(invoke->isNormalMethodCall()) {
    } else {
    }
}

void CodeGenerator::traverseAndGenerate(ArgumentsStar* args) {
    if(!args->isEpsilon()) {
        traverseAndGenerate(args->getListOfArguments());
    }
}

void CodeGenerator::traverseAndGenerate(Arguments* arg) {
    if(!arg->lastArgument()) {
        traverseAndGenerate(arg->getNextArgs());
    }
}

void CodeGenerator::traverseAndGenerate(NewClassCreation* create) {
    traverseAndGenerate(create->getArgsToCreateClass());
}

void CodeGenerator::traverseAndGenerate(PrimaryNewArray* newArray) {
    traverseAndGenerate(newArray->getTheDimension());
}
