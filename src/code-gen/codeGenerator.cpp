#include <cassert>

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
#include "primaryExpression.h"
#include "nameExpression.h"
#include "qualifiedThis.h"
#include "assignName.h"
#include "assignField.h"
#include "assignArray.h"
#include "literalOrThis.h"
#include "negationExpression.h"
#include "castName.h"
#include "castPrimitive.h"
#include "instanceof.h"
#include "blockStmtsStar.h"
#include "localDecl.h"
#include "ifStmt.h"
#include "whileStmt.h"
#include "forStmt.h"
#include "returnStmt.h"
#include "stmtExprAssign.h"
#include "stmtExprInvoke.h"
#include "stmtExprCreation.h"
#include "nestedBlock.h"
#include "expressionStar.h"

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

void CodeGenerator::traverseAndGenerate(Expression* expr) {
    if(expr->isRegularBinaryExpression()) {
        traverseAndGenerate((BinaryExpression*) expr);
    } else if(expr->isPrimaryExpression()) {
        traverseAndGenerate(((PrimaryExpression*) expr)->getPrimaryExpression());
    } else if(expr->isNameExpression()) {
        traverseAndGenerate(((NameExpression*) expr)->getNameExpression());
    } else if(expr->isNumericNegation() || expr->isBooleanNegation()) {
        traverseAndGenerate((NegationExpression*) expr);
    } else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
        traverseAndGenerate((CastExpression*) expr);
    } else if(expr->isInstanceOf()) {
        traverseAndGenerate((InstanceOf*) expr);
    } else {
        // assignment
        traverseAndGenerate((Assignment*) expr);
    }
}

void CodeGenerator::traverseAndGenerate(BinaryExpression* binExpr) {
    // Order based on JLS 15.7.2, except || (azy or) and && (lazy and)
    traverseAndGenerate(binExpr->getLeftExpression());
    if(!binExpr->isLazyOr() && binExpr->isLazyAnd()) {
        traverseAndGenerate(binExpr->getRightExpression());
    }

    if(binExpr->isLazyOr()) {
        // Order based on JLS 15.23
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isLazyAnd()) {
        // Order based on JLS 15.24
        traverseAndGenerate(binExpr->getRightExpression());
    } else if(binExpr->isEagerOr()) {
        // Specific: JLS 15.22.2
    } else if(binExpr->isEagerAnd()) {
        // Specific: JLS 15.22.2
    } else if(binExpr->isEqual()) {
        // Specific: JLS 15.21
    } else if(binExpr->isNotEqual()) {
        // Specific: JLS 15.21
    } else if(binExpr->isLT()) {
        // Specific: JLS 15.20
    } else if(binExpr->isLTE()) {
        // Specific: JLS 15.20  
    } else if(binExpr->isGT()) {
        // Specific: JLS 15.20
    } else if(binExpr->isGTE()) {
        // Specific: JLS 15.20
    } else if(binExpr->isAddition()) {
        // Specific: JLS 15.18
    } else if(binExpr->isMinus()) {
        // Specific: JLS 15.18
    } else if(binExpr->isMultiplication()) {
        // Specific: JLS 15.17
    } else if(binExpr->isDivision()) {
        // Specific: JLS 15.17
    } else {
        // Specific: JLS 15.17
        // modulo
        assert(binExpr->isModulo());
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
        traverseAndGenerate((PrimaryNewArray*) prim);
    } else if(prim->isQualifiedThis()) {
        traverseAndGenerate((QualifiedThis*) prim);
    } else {
        // LiteralOrThis
        traverseAndGenerate((LiteralOrThis*) prim);
    }
}

void CodeGenerator::traverseAndGenerate(ArrayAccess* access) {
    // Order based on JLS 15.13
    if(access->isArrayAccessName()) {
        traverseAndGenerate(((ArrayAccessName*) access)->getNameOfAccessedArray());
    } else {
        traverseAndGenerate(((ArrayAccessPrimary*) access)->getAccessedPrimaryArray());
    }
    traverseAndGenerate(access->getAccessExpression());
}

void CodeGenerator::traverseAndGenerate(Name* name) {
    // Order implicit based on JLS 15.7
    if(!name->isLastPrefix()) {
        traverseAndGenerate(name->getNextName());
    }
}

void CodeGenerator::traverseAndGenerate(FieldAccess* access) {
    // Order based on JLS 15.11.1
    traverseAndGenerate(access->getAccessedFieldPrimary());
}

void CodeGenerator::traverseAndGenerate(MethodInvoke* invoke) {
    // Order based on JLS 15.12.4
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
    // Left to right on the stack, with this at the top
    if(!arg->lastArgument()) {
        traverseAndGenerate(arg->getNextArgs());
    }
}

void CodeGenerator::traverseAndGenerate(NewClassCreation* create) {
    // Order based on JLS 15.9.4
    traverseAndGenerate(create->getArgsToCreateClass());
}

void CodeGenerator::traverseAndGenerate(PrimaryNewArray* newArray) {
    // Order based on JLS 15.10.1
    traverseAndGenerate(newArray->getTheDimension());
}

void CodeGenerator::traverseAndGenerate(QualifiedThis* qual) {
    // JLS 15.8.4
}

void CodeGenerator::traverseAndGenerate(LiteralOrThis* lit) {
    if(lit->isThis()) {
        // JLS 15.8.3
    } else if(lit->isNumber()) {
        // JLS 15.8.1
    } else if(lit->isTrueBoolean()) {
        // JLS 15.8.1
    } else if(lit->isFalseBoolean()) {
        // JLS 15.8.1
    } else if(lit->isCharLiteral()) {
        // JLS 15.8.1
    } else if(lit->isStringLiteral()) {
        // JLS 15.8.1
    } else {
        // JLS 15.8.1
        // precautionary check
        assert(lit->isNull());
    }
}

void CodeGenerator::traverseAndGenerate(NegationExpression* negExpr) {
    // Order based on JLS 15.7
    traverseAndGenerate(negExpr->getNegatedExpression());
    if(negExpr->isNumericNegation()) {
        // Specific: JLS 15.18.2
    } else {
        // Specific: JLS 15.15.6
        // boolean negation
    }
}

void CodeGenerator::traverseAndGenerate(CastExpression* cast) {
    // Order based on JLS 15.7
    // Specific: JLS 15.20.2 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(cast->getExpressionToCast());
    if(cast->isCastToPrimitiveType()) {
    } else {
        // cast to array or some reference type
    }
}

void CodeGenerator::traverseAndGenerate(InstanceOf* instanceof) {
    // Order based on JLS 15.7
    // Specific: JLS 15.20.2 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(instanceof->getExpressionToCheck());
}

void CodeGenerator::traverseAndGenerate(Assignment* assign) {
    // Order based on JLS 15.7
    // Specific: JLS 15.26
    if(assign->isAssignName()) {
        traverseAndGenerate(((AssignName*) assign)->getNameToAssign());
    } else if(assign->isAssignField()) {
        traverseAndGenerate(((AssignField*) assign)->getAssignedField());
    } else {
        // assigning to an array component
        traverseAndGenerate(((AssignArray*) assign)->getAssignedArray());
    }
    
    traverseAndGenerate(assign->getExpressionToAssign());
}

void CodeGenerator::traverseAndGenerate(ClassMethod* method) {
    if(!method->getMethodBody()->noDefinition()) {
        // the method has a body, then generate code
        // for the body
        traverseAndGenerate(method->getMethodBody());
    }
}

void CodeGenerator::traverseAndGenerate(MethodBody* body) {
    traverseAndGenerate(body->getBlockStmtsStar());
}

void CodeGenerator::traverseAndGenerate(BlockStmtsStar* stmts) {
    if(!stmts->isEpsilon()) {
        traverseAndGenerate(stmts->getStatements());
    }
}

void CodeGenerator::traverseAndGenerate(BlockStmts* stmt) {
    if(!stmt->isLastStatement()) {
        traverseAndGenerate(stmt->getNextBlockStmt());
    }

    if(stmt->isLocalVarDecl()) {
        traverseAndGenerate((LocalDecl*) stmt);
    } else if(stmt->isIfStmt() || stmt->isIfThenElseStmt()) {
        traverseAndGenerate((IfStmt*) stmt);
    } else if(stmt->isWhileStmt()) {
        traverseAndGenerate((WhileStmt*) stmt);
    } else if(stmt->isForStmt()) {
        traverseAndGenerate((ForStmt*) stmt);
    } else if(stmt->isAssignStmt() || stmt->isClassCreationStmt() || stmt->isMethodInvokeStmt()) {
        traverseAndGenerate((StmtExpr*) stmt);
    } else if(stmt->isNestedBlock()) {
        traverseAndGenerate((NestedBlock*) stmt);
    } else if(stmt->isReturnStmt()) {
        traverseAndGenerate((ReturnStmt*) stmt);
    }
}

void CodeGenerator::traverseAndGenerate(LocalDecl* local) {
    // Order based on JLS 14.4.4
    traverseAndGenerate(local->getLocalInitExpr());
}

void CodeGenerator::traverseAndGenerate(IfStmt* stmt) {
    // Order based on JLS 14.9
    traverseAndGenerate(stmt->getExpressionToEvaluate());
    traverseAndGenerate(stmt->getExecuteTruePart());
    if(!stmt->noElsePart()) {
        traverseAndGenerate(stmt->getExecuteFalsePart());
    }
}

void CodeGenerator::traverseAndGenerate(WhileStmt* stmt) {
    // Order based on JLS 14.11
    traverseAndGenerate(stmt->getExpressionToEvaluate());
    traverseAndGenerate(stmt->getLoopStmt());
}

void CodeGenerator::traverseAndGenerate(ForStmt* stmt) {
    // Order based on JLS 14.13.2
    if(!stmt->emptyForInit()) {
        traverseAndGenerate(stmt->getForInit());
    }

    traverseAndGenerate(stmt->getForUpdate());
    traverseAndGenerate(stmt->getLoopStmt());

    if(!stmt->emptyForUpdate()) {
        traverseAndGenerate(stmt->getForUpdate());
    }
}

void CodeGenerator::traverseAndGenerate(ExpressionStar* exprStar) {
    if(!exprStar->isEpsilon()) {
        traverseAndGenerate(exprStar->getExpression());
    }
}

void CodeGenerator::traverseAndGenerate(StmtExpr* stmt) {
    if(stmt->isAssignStmt()) {
        traverseAndGenerate(((StmtExprAssign*) stmt)->getAssignmentExpression());
    } else if(stmt->isClassCreationStmt()) {
        traverseAndGenerate(((StmtExprCreation*) stmt)->getCreatedClass());
    } else {
        // method invocation
        traverseAndGenerate(((StmtExprInvoke*) stmt)->getMethodInvoked());
    }
}

void CodeGenerator::traverseAndGenerate(NestedBlock* stmt) {
    // JLS 14.2
    if(!stmt->isEmptyNestedBlock()) {
        traverseAndGenerate(stmt->getNestedBlock());
    }
}

void CodeGenerator::traverseAndGenerate(ReturnStmt* stmt) {
    // JLS 14.16
    traverseAndGenerate(stmt->getReturnExpr());
}

void CodeGenerator::traverseAndGenerate(Constructor* ctor) {
    traverseAndGenerate(ctor->getConstructorBody());
}
