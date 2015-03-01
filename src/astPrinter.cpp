#include <iostream>
#include "astPrinter.h"
#include <cassert>

unsigned int AstPrinter::depth = 0;

void AstPrinter::printSpaces() {
    for(unsigned int i = 0; i < depth; i++) {
        std::cout << ' ';
    }
}

void AstPrinter::print(TypeDecl& node) {
    if(node.isClass()) {
        print((ClassDecl&) node);
    } else if(node.isInterface()) {
        print((InterfaceDecl&) node);
    }
}

void AstPrinter::print(ClassBodyDecls& node) {
    if(node.isField()) {
        print((FieldDecl&) node);
    } else if(node.isClassMethod()) {
        print((ClassMethod&) node);
    } else {
        assert(node.isConstructor());
        print((Constructor&) node);
    }
    depth++;
    if(!node.isLastClassMember()) {
        print(*node.getNextDeclaration());
    }
    depth--;
}

void AstPrinter::print(Expression& node) {
    if(node.isLazyOr() || node.isLazyAnd() || node.isEagerOr() || node.isEagerAnd() || node.isEqual() ||
       node.isNotEqual() || node.isLT() || node.isGT() || node.isLTE() || node.isGTE() || node.isAddition() ||
       node.isMinus() || node.isMultiplication() || node.isDivision() || node.isModulo()) {
        print((BinaryExpression&) node);
    } else if(node.isPrimaryExpression()) {
        print((PrimaryExpression&) node);
    } else if(node.isCastToReferenceType() || node.isCastToArrayName() || node.isCastToPrimitiveType()) {
        print((CastExpression&) node);
    } else if(node.isInstanceOf()) {
        print((InstanceOf&) node);
    } else if(node.isNumericNegation() || node.isBooleanNegation()) {
        print((NegationExpression&) node);
    } else if(node.isNameExpression()) {
        print((NameExpression&) node);
    } else {
        assert(node.isAssignName() || node.isAssignField() || node.isAssignArray());
        print((Assignment&) node);
    }
}

void AstPrinter::print(Type& node) {
    if(node.isPrimitiveType()) {
        print((PrimitiveType&) node);
    } else {
        assert(node.isReferenceType());
        print((ReferenceType&) node);
    }
}

void AstPrinter::print(Primary& node) {
    if(node.isArrayAccessName() || node.isArrayAccessPrimary()) {
        print((ArrayAccess&) node);
    } else if(node.isBracketedExpression()) {
        print((BracketedExpression&) node);
    } else if(node.isFieldAccess()) {
        print((FieldAccess&) node);
    } else if(node.isNormalMethodCall() || node.isAccessedMethodCall()) {
        print((MethodInvoke&) node);
    } else if(node.isNewClassCreation()) {
        print((NewClassCreation&) node);
    } else if(node.isThis() || node.isNumber() || node.isTrueBoolean() ||
              node.isFalseBoolean() || node.isCharLiteral() || node.isStringLiteral()) {
        print((LiteralOrThis&) node);
    } else if(node.isQualifiedThis()) {
        print((QualifiedThis&) node);
    } else {
        std::cout << node.getRule() << std::endl;
        assert(node.isNewPrimitiveArray() || node.isNewReferenceArray());
        print((PrimaryNewArray&) node);
    }
}

void AstPrinter::print(MethodInvoke& node) {
    if(node.isNormalMethodCall()) {
        print((MethodNormalInvoke&) node);
    } else {
        assert(node.isAccessedMethodCall());
        print((InvokeAccessedMethod&) node);
    }
    depth++;
    print(*node.getArgsForInvokedMethod());
    depth--;
}

void AstPrinter::print(CastExpression& node) {
    if(node.isCastToPrimitiveType()) {
        print((CastPrimitive&) node);
    } else {
        assert(node.isCastToReferenceType() || node.isCastToArrayName());
        print((CastName&) node);
    }
    depth++;
    print(*node.getExpressionToCast());
    depth--;
}

void AstPrinter::print(Assignment& node) {
    if(node.isAssignName()) {
        print((AssignName&) node);
    } else if(node.isAssignField()) {
        print((AssignField&) node);
    } else {
        assert(node.isAssignArray());
        print((AssignArray&) node);
    }
    depth++;
    print(*node.getExpressionToAssign());
    depth--;
}

void AstPrinter::print(ArrayAccess& node) {
    if(node.isArrayAccessName()) {
        print((ArrayAccessName&) node);
    } else {
        assert(node.isArrayAccessPrimary());
        print((ArrayAccessPrimary&) node);
    }
    depth++;
    print(*node.getAccessExpression());
    depth--;
}

void AstPrinter::print(BlockStmts& node) {
    if(node.isEmptyStmt()) {
        print((EmptyStmt&) node);
    } else if(node.isForStmt()) {
        print((ForStmt&) node);
    } else if(node.isAssignStmt() || node.isClassCreationStmt() || node.isMethodInvokeStmt()) {
        print((StmtExpr&) node);
    } else if(node.isIfStmt() || node.isIfThenElseStmt()) {
        print((IfStmt&) node);
    } else if(node.isLocalVarDecl()) {
        print((LocalDecl&) node);
    } else if(node.isNestedBlock()) {
        print((NestedBlock&) node);
    } else if(node.isReturnStmt()) {
        print((ReturnStmt&) node);
    } else {
        assert(node.isWhileStmt());
        print((WhileStmt&) node);
    }
    depth++;
    if(!node.isLastStatement()) {
        print(*node.getNextBlockStmt());
    }
    depth--;
}

void AstPrinter::print(StmtExpr& node) {
    if(node.isAssignStmt()) {
        print((StmtExprAssign&) node);
    } else if(node.isClassCreationStmt()) {
        print((StmtExprCreation&) node);
    } else {
        print((StmtExprInvoke&) node);
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Class

void AstPrinter::print(CompilationUnit& node) {
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getPackageDecl());
    print(*node.getImportDeclsStar());
    print(*node.getTypeDecl());
}

void AstPrinter::print(PackageDecl& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getPackageName());
    }
    depth--;
}

void AstPrinter::print(ImportDeclsStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getImportDeclarations());
    }
    depth--;
}

void AstPrinter::print(Name& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << ':' << node.getFullName() << std::endl;
    depth--;
}

void AstPrinter::print(Identifier& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << ':' << node.getIdAsString() << std::endl;
    depth--;
}

void AstPrinter::print(ImportDecls& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    assert(node.isSingleImport() || node.isOnDemandImport());
    print(*node.getCurrentImport());
    if(!node.isLastImport()) { print(*node.getNextImport()); }
    depth--;
}

void AstPrinter::print(ClassDecl& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getClassModifiers());
    print(*node.getClassId());
    print(*node.getSuper());
    print(*node.getImplementInterfaces());
    print(*node.getClassMembers());
    depth--;
}

void AstPrinter::print(Modifiers& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << ':' << node.getCurrentModifierAsString() << std::endl;
    if(!node.isLastModifier()) {
        print(*node.getNextModifier());
    }
    depth--;
}

void AstPrinter::print(Super& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getSuperName());
    }
    depth--;
}

void AstPrinter::print(InterfaceList& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getListOfInterfaces());
    }
    depth--;
}

void AstPrinter::print(Interfaces& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getCurrentInterface());
    if(!node.isLastInterface()) {
        print(*node.getNextInterface());
    }
    depth--;
}

void AstPrinter::print(ClassBodyStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getBody());
    }
    depth--;
}

void AstPrinter::print(FieldDecl& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getModifiers());
    print(*node.getFieldType());
    print(*node.getFieldDeclared());
    if(node.isInitialized()) {
        print(*node.getInitializingExpression());
    }
    depth--;
}

void AstPrinter::print(PrimitiveType& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << ':' << node.getTypeAsString() << std::endl;
    depth--;
}

void AstPrinter::print(ReferenceType& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getReferenceName());
    depth--;
}

void AstPrinter::print(BinaryExpression& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getLeftExpression());
    print(*node.getRightExpression());
    depth--;
}

void AstPrinter::print(LiteralOrThis& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << ':' << node.getLiteralAsString() << std::endl;
    depth--;
}

void AstPrinter::print(BracketedExpression& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getExpressionInside());
    depth--;
}

void AstPrinter::print(PrimaryExpression& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getPrimaryExpression());
    depth--;
}

void AstPrinter::print(MethodNormalInvoke& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNameOfInvokedMethod());
    depth--;
}

void AstPrinter::print(InvokeAccessedMethod& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ';' << node.getRule() << std::endl;
    print(*node.getAccessedMethod());
    depth--;
}

void AstPrinter::print(ArgumentsStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getListOfArguments());
    }
    depth--;
}

void AstPrinter::print(Arguments& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getSelfArgumentExpr());
    if(!node.lastArgument()) {
        print(*node.getNextArgs());
    }
    depth--;
}

void AstPrinter::print(NewClassCreation& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getClassName());
    print(*node.getArgsToCreateClass());
    depth--;
}

void AstPrinter::print(PrimaryNewArray& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getArrayType());
    print(*node.getTheDimension());
    depth--;
}

void AstPrinter::print(CastPrimitive& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getPrimitiveTypeToCastTo());
    depth--;
}

void AstPrinter::print(CastName& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNameToCastTo());
    depth--;
}

void AstPrinter::print(InstanceOf& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getExpressionToCheck());
    print(*node.getTypeToCheck());
    depth--;
}

void AstPrinter::print(NameExpression& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNameExpression());
    depth--;
}

void AstPrinter::print(NegationExpression& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNegatedExpression());
    depth--;
}

void AstPrinter::print(AssignName& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNameToAssign());
    depth--;
}

void AstPrinter::print(AssignArray& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getAssignedArray());
    depth--;
}

void AstPrinter::print(ArrayAccessName& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getNameOfAccessedArray());
    depth--;
}

void AstPrinter::print(ArrayAccessPrimary& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getAccessedPrimaryArray());
    depth--;
}

void AstPrinter::print(AssignField& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getAssignedField());
    depth--;
}

void AstPrinter::print(FieldAccess& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getAccessedFieldPrimary());
    print(*node.getAccessedFieldId());
    depth--;
}

void AstPrinter::print(ClassMethod& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getModifiers());
    print(*node.getMethodHeader());
    print(*node.getMethodBody());
    depth--;
}

void AstPrinter::print(MethodHeader& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getClassMethodId());
    if(!node.isVoidReturnType()) {
        print(*node.getReturnType());
    }
    print(*node.getClassMethodParams());
    depth--;
}

void AstPrinter::print(FormalParamStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getListOfParameters());
    }
    depth--;
}

void AstPrinter::print(ParamList& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getParameterType());
    print(*node.getParameterId());
    if(!node.isLastParameter()) {
        print(*node.getNextParameter());
    }
    depth--;
}

void AstPrinter::print(MethodBody& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.noDefinition()) {
        print(*node.getBlockStmtsStar());
    }
    depth--;
}

void AstPrinter::print(BlockStmtsStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getStatements());
    }
    depth--;
}

void AstPrinter::print(EmptyStmt& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    depth--;
}

void AstPrinter::print(ForStmt& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.emptyForInit()) {
        print(*node.getForInit());
    }
    print(*node.getExpressionToEvaluate());
    if(!node.emptyForUpdate()) {
        print(*node.getForUpdate());
    }
    print(*node.getLoopStmt());
    depth--;
}

void AstPrinter::print(ExpressionStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getExpression());
    }
    depth--;
}

void AstPrinter::print(StmtExprAssign& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getAssignmentExpression());
    depth--;
}

void AstPrinter::print(StmtExprCreation& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getCreatedClass());
    depth--;
}

void AstPrinter::print(StmtExprInvoke& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getMethodInvoked());
    depth--;
}

void AstPrinter::print(IfStmt& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getExpressionToEvaluate());
    print(*node.getExecuteTruePart());
    if(!node.noElsePart()) {
        print(*node.getExecuteFalsePart());
    }
    depth--;
}

void AstPrinter::print(LocalDecl& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getLocalType());
    print(*node.getLocalId());
    print(*node.getLocalInitExpr());
    depth--;
}

void AstPrinter::print(NestedBlock& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEmptyNestedBlock()) {
        print(*node.getNestedBlock());
    }
    depth--;
}

void AstPrinter::print(ReturnStmt& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getReturnExpr());
    depth--;
}

void AstPrinter::print(WhileStmt& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getExpressionToEvaluate());
    print(*node.getLoopStmt());
    depth--;
}

void AstPrinter::print(Constructor& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getConstructorId());
    print(*node.getConstructorParameters());
    print(*node.getConstructorBody());
    depth--;
}

void AstPrinter::print(QualifiedThis& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getQualifyingClassName());
    depth--;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Interface

void AstPrinter::print(InterfaceDecl& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getInterfaceModifiers());
    print(*node.getInterfaceId());
    print(*node.getExtendedInterfaces());
    print(*node.getInterfaceBodyStar());
    depth--;
}

void AstPrinter::print(InterfaceBodyStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getInterfaceMethods());
    }
    depth--;
}

void AstPrinter::print(InterfaceMethod& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    print(*node.getModifiersStar());
    if(!node.isVoidReturnType()) {
        print(*node.getReturnType());
    }
    print(*node.getInterfaceMethodId());
    print(*node.getParametersList());
    if(!node.isLastMethod()) {
        print(*node.getNextInterfaceMethod());
    }
    depth--;
}

void AstPrinter::print(ModifiersStar& node) {
    depth++;
    printSpaces();
    std::cout << node.getLexeme() << ':' << node.getRule() << std::endl;
    if(!node.isEpsilon()) {
        print(*node.getListOfModifiers());
    }
    depth--;
}
