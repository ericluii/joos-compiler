#include "typeChecker.h"
#include "primitiveTypeConversions.h"
#include "error.h"
#include <queue>
#include <sstream>
#include <stack>
#include <cassert>

bool isPrimitive(std::string type) {
    return type == "int" || type == "short" || type == "byte" || type == "char" || type == "boolean" || type == "float" || type == "double";
}

bool isNumeric(std::string type) {
    return type == "int" || type == "short" || type == "byte" || type == "char" || type == "float" || type == "double";
}

bool isPrimitiveArray(std::string type) {
    return type == "int[]" || type == "short[]" || type == "byte[]" || type == "char[]" || type == "boolean[]";
}

bool isArray(std::string type) {
    return type.find("[]") != std::string::npos;
}

std::string getQualifierFromString(std::string str) {
    std::string qualifier;
    if (str.find('.') != std::string::npos) {
        return str.substr(0, str.find_last_of('.'));
    } else {
        return "";
    }
}

TypeChecking::TypeChecking(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& packages) :
    manager(manager),
    packages(packages),
    cur_st_type(NONE),
    restrict_this(false),
    restrict_num(false),
    restrict_type_name_expressions(false),
    static_context_only(false),
    last_type_to_type_error("")
{}

void TypeChecking::check() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            if (!check(*it2)) {
                return;
            }
        }
    }
}

bool TypeChecking::check(CompilationTable* compilation) {
    processing = compilation;
    SymbolTable* st = compilation->getSymbolTable();

    if (st == NULL || !st->isClassTable()) {
        return true;
    }

    CHECK_PUSH(static_cast<ClassTable*>(st)->getClass(), st, CLASS_TABLE);
}

bool TypeChecking::check(ClassDecl* classDecl) {
    if (classDecl->emptyBody()) {
        return true;
    }

    return check(classDecl->getClassMembers());
}

bool TypeChecking::check(ClassBodyStar* classBodyStar) {
    if (classBodyStar->isEpsilon()) {
        return true;
    }

    return check(classBodyStar->getBody());
}

bool TypeChecking::check(ClassBodyDecls* classBodyDecls) {
    bool rest_of_body = true;
    if (!classBodyDecls->isLastClassMember()) {
        if (!check(classBodyDecls->getNextDeclaration())) {
            return false;
        }
    }

    if (classBodyDecls->isClassMethod()) {
        SET_RETURN(static_cast<ClassMethod*>(classBodyDecls), restrict_this, classBodyDecls->isStatic());
    } else if (classBodyDecls->isConstructor()) {
        return check(static_cast<Constructor*>(classBodyDecls));
    } else if (classBodyDecls->isField()) {
        SET_RETURN(static_cast<FieldDecl*>(classBodyDecls), restrict_this, classBodyDecls->isStatic());
    }

    assert(false);
    return true;
}

bool TypeChecking::check(FieldDecl* fieldDecl) {
    if (!fieldDecl->isInitialized()) {
        return true;
    }

    if (!canAssignTypeToType(fieldDecl->getFieldType()->getTypeAsString(), fieldDecl->getInitializingExpression()->getExpressionTypeString())) {
        // TODO: CREATE TESTS FOR BAD FIELD DECLS
        std::stringstream ss;
        ss << last_type_to_type_error;
        NOTIFY_ERROR(fieldDecl->getFieldDeclared()->getToken(), ss);
    }

    CHECK_PUSH_AND_SET(fieldDecl->getInitializingExpression(), fieldDecl->getFieldTable(), FIELDDECL_TABLE,
                           static_context_only, fieldDecl->isStatic());
}

bool TypeChecking::check(ClassMethod* classMethod) {
    CHECK_PUSH(classMethod->getMethodBody(), classMethod->getClassMethodTable(), CLASSMETHOD_TABLE);
}

bool TypeChecking::check(Constructor* constructor) {
    if (!constructor->emptyConstructorBody()) {
        CHECK_PUSH(constructor->getConstructorBody(), constructor->getConstructorTable(), CONSTRUCTOR_TABLE);
    }

    // Check against impilicit super() call
    ClassDecl* cd = static_cast<ClassTable*>(st_stack.top())->getClass();
    if (!cd->noSuperClass()) {
        CompilationTable* ct = cd->getSuper()->getSuperClassTable();
        if (!ct->getAConstructor("()")) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] No default constructor in super class.");
            return false;
        }
    }

    return true;
}

bool TypeChecking::check(MethodBody* methodBody) {
    if (methodBody->noDefinition() || methodBody->emptyClassMethodBody()) {
        return true;
    }

    return check(methodBody->getBlockStmtsStar());
}

bool TypeChecking::check(BlockStmtsStar* blockStmtsStar) {
    if (blockStmtsStar->isEpsilon()) {
        return true;
    }

    return check(blockStmtsStar->getStatements());
}

bool TypeChecking::check(BlockStmts* blockStmts) {
    bool rest_of_statements = true;
    if (!blockStmts->isLastStatement()) {
        rest_of_statements = check(blockStmts->getNextBlockStmt());
    }

    if (blockStmts->isLocalVarDecl()) {
        return check(static_cast<LocalDecl*>(blockStmts)) && rest_of_statements;
    } else if (blockStmts->isReturnStmt()) {
        return check(static_cast<ReturnStmt*>(blockStmts)) && rest_of_statements;
    } else if (blockStmts->isAssignStmt()) {
        return check(static_cast<StmtExprAssign*>(blockStmts)->getAssignmentExpression()) && rest_of_statements;
    } else if (blockStmts->isMethodInvokeStmt()) {
        return check(static_cast<StmtExprInvoke*>(blockStmts)->getMethodInvoked()) && rest_of_statements;
    } else if (blockStmts->isClassCreationStmt()) {
        return check(static_cast<StmtExprCreation*>(blockStmts)->getCreatedClass()) && rest_of_statements;
    } else if (blockStmts->isIfStmt() || blockStmts->isIfThenElseStmt()) {
        return check(static_cast<IfStmt*>(blockStmts)) && rest_of_statements;
    } else if (blockStmts->isWhileStmt()) {
        return check(static_cast<WhileStmt*>(blockStmts)) && rest_of_statements;
    } else if (blockStmts->isForStmt()) {
        return check(static_cast<ForStmt*>(blockStmts)) && rest_of_statements;
    } else if (blockStmts->isNestedBlock()) {
        return check(static_cast<NestedBlock*>(blockStmts)) && rest_of_statements;
    }

    assert(false);
    return true;
}

bool TypeChecking::check(IfStmt* ifStmt) {
    bool else_result = true;
    if (!ifStmt->noElsePart()) {
        else_result = check(ifStmt->getExecuteFalsePart());
    }

    if (ifStmt->getExpressionToEvaluate()->getExpressionTypeString() != "boolean") {
        Error(E_DEFAULT, NULL, "Cannot have a non-boolean value as a condition.");
        return false;
    }

    return check(ifStmt->getExpressionToEvaluate()) && check(ifStmt->getExecuteTruePart()) && else_result;
}

bool TypeChecking::check(WhileStmt* whileStmt) {
    if (whileStmt->getExpressionToEvaluate()->getExpressionTypeString() != "boolean") {
        Error(E_DEFAULT, NULL, "Cannot have a non-boolean value as a condition.");
        return false;
    }

    return check(whileStmt->getExpressionToEvaluate()) && check(whileStmt->getLoopStmt());
}

bool TypeChecking::check(ForStmt* forStmt) {
    bool init_result = true;
    bool expression_result = true;
    bool update_result = true;

    if (!forStmt->emptyForInit()) {
        init_result = check(forStmt->getForInit());
    }

    if (!forStmt->emptyExpression()) {
        if (!forStmt->getExpressionToEvaluate()->isEpsilon()) {
            if (forStmt->getExpressionToEvaluate()->getExpression()->getExpressionTypeString() != "boolean") {
                Error(E_DEFAULT, NULL, "Cannot have a non-boolean value as a condition.");
                return false;
            }
        }
        
        expression_result = check(forStmt->getExpressionToEvaluate());
    }

    if (!forStmt->emptyForUpdate()) {
        update_result = check(forStmt->getForUpdate());
    }

    return  init_result && expression_result && update_result && check(forStmt->getLoopStmt());
}

bool TypeChecking::check(StmtExpr* stmtExpr) {
    if (stmtExpr->isAssignStmt()) {
        return check(static_cast<StmtExprAssign*>(stmtExpr)->getAssignmentExpression());
    } else if (stmtExpr->isClassCreationStmt()) {
        return check(static_cast<StmtExprCreation*>(stmtExpr)->getCreatedClass());
    } else {
        return check(static_cast<StmtExprInvoke*>(stmtExpr)->getMethodInvoked());
    }
}

bool TypeChecking::check(ExpressionStar* expressionStar) {
    if (!expressionStar->isEpsilon()) {
        return check(expressionStar->getExpression());
    }

    return true;
}

bool TypeChecking::check(NestedBlock* nestedBlock) {
    if (!nestedBlock->isEmptyNestedBlock()) {
        return check(nestedBlock->getNestedBlock());
    }

    return true;
}

bool TypeChecking::check(MethodInvoke* methodInvoke) {
    if (methodInvoke->isNormalMethodCall()) {
        Name* name = static_cast<MethodNormalInvoke*>(methodInvoke)->getNameOfInvokedMethod();

        canAccessName(name, static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());

        return check(methodInvoke->getArgsForInvokedMethod());
    } else {

        // TODO: CHECK SOMETHING...

        return check(static_cast<InvokeAccessedMethod*>(methodInvoke)->getAccessedMethod()) && check(methodInvoke->getArgsForInvokedMethod());
    }
}

bool TypeChecking::check(NewClassCreation* newClassCreation) {
    // TODO: CHECK IF CALLED CONSTRUCTOR IS PROTECTED
    CompilationTable* table = newClassCreation->getTableOfCreatedClass();
    if (table->isClassSymbolTable()) {
        ClassTable* ct = static_cast<ClassTable*>(table->getSymbolTable());
        ClassDecl* cd = ct->getClass();

        if (cd->isAbstract()) {
            std::stringstream ss;
            ss << "Abstract class '" << cd->getClassId()->getIdAsString() << "' cannot be constructed.";
            NOTIFY_ERROR(newClassCreation->getClassName()->getNameId()->getToken(), ss);
        }
    }

    return check(newClassCreation->getArgsToCreateClass());
}

bool TypeChecking::check(ArgumentsStar* argumentsStar) {
    if (argumentsStar->isEpsilon()) {
        return true;
    }

    return check(argumentsStar->getListOfArguments());
}

bool TypeChecking::check(Arguments* arguments) {
    bool rest_of_arguements = true;
    if (!arguments->lastArgument()) {
        rest_of_arguements = check(arguments->getNextArgs());
    }

    return rest_of_arguements && check(arguments->getSelfArgumentExpr());
}

// JLS 14.16
bool TypeChecking::check(ReturnStmt* returnStmt) {
    if (returnStmt->isEmptyReturn()) {
        // JLS 8.4.5 - A method with a return type cannot return with no expression
        if (cur_st_type == CLASSMETHOD_TABLE) {
            MethodHeader* mh = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader();

            if (!mh->isVoidReturnType()) {
                std::stringstream ss;
                ss << "Method '" << mh->methodSignatureAsString() << "' is returning void, but has "
                   << "a non-void return type.";
                NOTIFY_ERROR(mh->getClassMethodId()->getToken(), ss);
            }
        }

        return true;
    } else {
        // JLS 8.8.5 - Contructor Body cannot return an expression.
        if (cur_st_type == CONSTRUCTOR_TABLE) {
            Constructor* constructor = static_cast<ConstructorTable*>(st_stack.top())->getConstructor();
            Token* token = constructor->getConstructorId()->getToken();

            std::stringstream ss;
            ss << "Constructor '" << token->getString() << constructor->constructorSignatureAsString()
               << "' cannot have a non void return type.";
            NOTIFY_ERROR(token, ss);
        }

        if (cur_st_type == CLASSMETHOD_TABLE) {
            MethodHeader* mh = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader();

            // JLS 8.4.5 - A method that is declared void cannot have a non-empty expression returned
            if (mh->isVoidReturnType()) {
                std::stringstream ss;
                ss << "Method '" << mh->methodSignatureAsString() << "' is returning non-void, but has "
                   << "a void return type.";
                NOTIFY_ERROR(mh->getClassMethodId()->getToken(), ss);
            }

            // JLS 14.16 && 5.2 - The type T must be assignable to the declared result type of the method.
            if (!mh->isVoidReturnType() &&
                !canAssignTypeToType(mh->getReturnType()->getTypeAsString(),
                                     returnStmt->getReturnExpr()->getExpression()->getExpressionTypeString())) {
                std::stringstream ss;
                ss << last_type_to_type_error;
                NOTIFY_ERROR(mh->getClassMethodId()->getToken(), ss);
            }
        }

        return check(returnStmt->getReturnExpr()->getExpression());
    }
}

bool TypeChecking::check(Expression* expression) {
    if (expression->getExpressionTypeString() == "void") {
        Error(E_DEFAULT, NULL, "Doing something with void. Stop It.");
        return false;
    }

    if(expression->isRegularBinaryExpression()) {
        // Something more than just NULL is happening
        restrict_num = false;

        Expression* leftExpr = static_cast<BinaryExpression*>(expression)->getLeftExpression();
        Expression* rightExpr = static_cast<BinaryExpression*>(expression)->getRightExpression();

        // JLS 15.21.3
        // A compile-time error occurs if it is impossible to convert the type of either operand
        // to the type of the other by a casting conversion
        if ((expression->isEqual() || expression->isNotEqual()) &&
            !canAssignTypeToType(leftExpr->getExpressionTypeString(), rightExpr->getExpressionTypeString()) &&
            !canAssignTypeToType(rightExpr->getExpressionTypeString(), leftExpr->getExpressionTypeString())) {
            Error(E_DEFAULT, NULL, "Cannot equal check.");
            return false;
        }

        if(expression->isEagerOr() || expression->isEagerAnd()) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Use of | or &.");
            return false;
        }

        // TODO: VALIDATE A WHOLE SHIT LOAD OF EXPRESSIONS

        return check(rightExpr) && check(leftExpr);
    } else if(expression->isAssignName() || expression->isAssignField() || expression->isAssignArray()) {
        return check(static_cast<Assignment*>(expression));
    } else if (expression->isInstanceOf()) {
        return check(static_cast<InstanceOf*>(expression));
    } else if (expression->isNameExpression()) {
        return check(static_cast<NameExpression*>(expression));
    } else if(expression->isCastToArrayName() || expression->isCastToReferenceType() || expression->isCastToPrimitiveType()) {
        check(static_cast<CastExpression*>(expression));
    } else if(expression->isPrimaryExpression()) {
        return check(static_cast<PrimaryExpression*>(expression)->getPrimaryExpression());
    } else if (expression->isNumericNegation() || expression->isBooleanNegation()) {
        restrict_num = expression->isBooleanNegation();
        bool rv = check(static_cast<NegationExpression*>(expression)->getNegatedExpression());
        restrict_num = false;
        return rv;
    }

    return true;
}

bool TypeChecking::check(NameExpression* nameExpression) {
    Name* name = nameExpression->getNameExpression();

    if (!canAccessName(name)) {
        // TODO: VALIDATE PROTECTED/STATIC ACCESS
    }

    return true;
}

// JLS 15.20.2
bool TypeChecking::check(InstanceOf* instanceOf) {
    // The type of a RelationalExpression operand of the instanceof operator must be a reference type or the null type
    if (instanceOf->getExpressionToCheck()->getExpressionTypeString() != "null" &&
        isPrimitive(instanceOf->getExpressionToCheck()->getExpressionTypeString())) {
        Error(E_DEFAULT, NULL, "Cannot check the instanceof a primitive type.");
        return false;
    }

    // The ReferenceType mentioned after the instanceof operator must denote a reference type
    if (isPrimitive(instanceOf->getTypeToCheck()->getTypeAsString())) {
        Error(E_DEFAULT, NULL, "Cannot check that a non-primitive is an instanceof a primitive type.");
        return false;
    }

    return check(instanceOf->getExpressionToCheck());
}

bool TypeChecking::check(Primary* primary) {
    if (primary->isFieldAccess()) {
        return check(static_cast<FieldAccess*>(primary));
    } else if(primary->isThis() || primary->isNumber() || primary->isTrueBoolean() || primary->isFalseBoolean()
              || primary->isCharLiteral() || primary->isStringLiteral() || primary->isNull()) {
        return check(static_cast<LiteralOrThis*>(primary)); 
    } else if (primary->isBracketedExpression()) {
        return check(static_cast<BracketedExpression*>(primary)->getExpressionInside());
    } else if (primary->isNewClassCreation()) {
        return check(static_cast<NewClassCreation*>(primary));
    } else if (primary->isNormalMethodCall() || primary->isAccessedMethodCall()) {
        return check(static_cast<MethodInvoke*>(primary));
    } else if (primary->isQualifiedThis()) {
        return check(static_cast<QualifiedThis*>(primary));
    } else if (primary->isArrayAccessName() || primary->isArrayAccessPrimary()) {
        return check(static_cast<ArrayAccess*>(primary));
    } else if (primary->isNewPrimitiveArray() || primary->isNewReferenceArray()) {
        return check(static_cast<PrimaryNewArray*>(primary));
    }

    assert(false);
    return true;
}

bool TypeChecking::check(PrimaryNewArray* primaryNewArray) {
    // JLS 10.4
    if (primaryNewArray->getTheDimension()->getExpressionTypeString() != "int" &&
        primaryNewArray->getTheDimension()->getExpressionTypeString() != "short" &&
        primaryNewArray->getTheDimension()->getExpressionTypeString() != "byte" &&
        primaryNewArray->getTheDimension()->getExpressionTypeString() != "char") {
        Error(E_DEFAULT, NULL, "Array cann only be indexed by an int, short, byte, or char.");
        return false;
    }

    return check(primaryNewArray->getTheDimension());
}

bool TypeChecking::check(ArrayAccess* arrayAccess) {
    // JLS 10.4
    if (arrayAccess->getAccessExpression()->getExpressionTypeString() != "int" &&
        arrayAccess->getAccessExpression()->getExpressionTypeString() != "short" &&
        arrayAccess->getAccessExpression()->getExpressionTypeString() != "byte" &&
        arrayAccess->getAccessExpression()->getExpressionTypeString() != "char") {
        Error(E_DEFAULT, NULL, "Array cann only be indexed by an int, short, byte, or char.");
        return false;
    }

    bool rv = check(arrayAccess->getAccessExpression());

    if (arrayAccess->isArrayAccessPrimary()) {
        return check(static_cast<ArrayAccessPrimary*>(arrayAccess)->getAccessedPrimaryArray()) && rv;
    } else {
        return rv;
    }
}

bool TypeChecking::check(QualifiedThis* qualifiedThis) {
    return true;
}

bool TypeChecking::check(FieldAccess* fieldAccess) {
    restrict_type_name_expressions = true;
    bool rv = check(fieldAccess->getAccessedFieldPrimary());
    restrict_type_name_expressions = false;

    return rv;
}

bool TypeChecking::check(CastExpression* castExpression){
    // TODO: CASTING CHECKS
    Expression* paramaterExpression = castExpression->getExpressionToCast();

    if(castExpression->isCastToArrayName()){
        return check(paramaterExpression);
    } else if(castExpression->isCastToReferenceType()){
        //std::cout << " to Reference type" << std::endl;
        if(isPrimitive(paramaterExpression->getExpressionTypeString()) || isPrimitiveArray(paramaterExpression->getExpressionTypeString()))
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast primitive to reference type");
            return false;
        }
        else
        {
            if(!inheritsOrExtendsOrImplements(paramaterExpression->getExpressionTypeString(), castExpression->getExpressionTypeString()) &&
               !inheritsOrExtendsOrImplements(castExpression->getExpressionTypeString(), paramaterExpression->getExpressionTypeString()) &&
               paramaterExpression->getExprType() != ET_NULL)
            {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] invalid reference type cast");
                return false;
            }
        }
    } else {
        //std::cout << " to Primitive type" << std::endl;
        if(!(isPrimitive(paramaterExpression->getExpressionTypeString()) || isPrimitiveArray(paramaterExpression->getExpressionTypeString())))
        {
            //std::cout << "Error on casting " << paramaterExpression->getExpressionTypeString() << " to Primitive type" << std::endl;
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast reference type to primitive type");
            return false;
        }
        else if(paramaterExpression->isExprTypeBoolean() && (castExpression->getExprType() != ET_BOOLEAN))
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a boolean");
            return false;
        }
        else if(isPrimitive(paramaterExpression->getExpressionTypeString()) && static_cast<CastPrimitive*>(castExpression)->isPrimitiveArrayCast())
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a primitive to a primitive array");
            return false;
        }
        else if(isPrimitiveArray(paramaterExpression->getExpressionTypeString()) && !static_cast<CastPrimitive*>(castExpression)->isPrimitiveArrayCast())
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a primitive array to a primitive");
            return false;
        }
    }

    return check(paramaterExpression);
}

bool TypeChecking::check(LiteralOrThis* literalOrThis) {
    // TODO: CHECK IF I CAN USE A LITERAL OR THIS...
    if (literalOrThis->isThis() && restrict_this) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] this in field init or static method.");
        return false;
        std::stringstream ss;
        ss << "this may be used only in the body of an instance method, instance initializer or constructor,"
           << "or in the initializer of an instance variable of a class.";
        NOTIFY_ERROR(literalOrThis->getLiteralToken(), ss);
    }

    return true;
}

bool TypeChecking::check(Assignment* assignment) {
    bool rv = true;

    if (!canAssignTypeToType(assignment->getExpressionTypeString(), assignment->getExpressionToAssign()->getExpressionTypeString())) {
        // TODO: Find Token
        // TODO: Add more assignment tests
        Error(E_DEFAULT, NULL, last_type_to_type_error);
        return false;
    } else if (assignment->isAssignName()) {
        Name* name = static_cast<AssignName*>(assignment)->getNameToAssign();

        if (!name->isSimpleName()) {
            std::string type = tryToGetTypename(name->getNextName(), processing);

            if (isArray(type) && name->getNameId()->getIdAsString() == "length") {
                std::stringstream ss;
                ss << "Cannot assign the the final field length of '" << name->getQualifier() << "'.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }
        }
    }

    return check(assignment->getExpressionToAssign()) && rv;
}

bool TypeChecking::check(LocalDecl* localDecl) {
    if (!canAssignTypeToType(localDecl->getLocalType()->getTypeAsString(), localDecl->getLocalInitExpr()->getExpressionTypeString())) {
        // TODO: CREATE TESTS FOR BAD LOCAL DECLS
        std::stringstream ss;
        ss << last_type_to_type_error;
        NOTIFY_ERROR(localDecl->getLocalId()->getToken(), ss);
    }

    return check(localDecl->getLocalInitExpr());
}

bool TypeChecking::canAccessName(Name* name, std::string methodSignature) {
    bool isMethod = methodSignature != "";

    // Simple Names can only be non-static contexts
    if (name->isSimpleName()) {
        // Name is either locally declared, a parameter, or a field declaration
        // We only need to check access permissions if it is a field declaration
        if (!localDeclOrParam(name, processing) && processing->getAField(name->getFullName())) {
            FieldDecl* fd = processing->getAField(name->getFullName())->getField();

            if (fd->isStatic()) {
                std::stringstream ss;
                ss << "Cannot access static variable '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }

            if (!fd->isStatic() && static_context_only) {
                std::stringstream ss;
                ss << "Cannot access static variable '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }

            if (!fd->isStatic() && restrict_this) {
                std::stringstream ss;
                ss << "'this' cannot be used implicitly in a static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }
        } else if (processing->getAClassMethod(methodSignature)) {
            ClassMethod* cm = processing->getAClassMethod(methodSignature)->getClassMethod();

            if (cm->isStatic()) {
                std::stringstream ss;
                ss << "Cannot access static method '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }

            if (!cm->isStatic() && static_context_only) {
                std::stringstream ss;
                ss << "Cannot access static method '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }

            if (!cm->isStatic() && restrict_this) {
                std::stringstream ss;
                ss << "'this' cannot be used implicitly in a static context.";
                NOTIFY_ERROR(name->getNameId()->getToken(), ss);
            }
        }
    } else {
        std::stack<Name*> traceback;

        while (name != NULL) {
            traceback.push(name);
            name = name->getNextName();
        }

        // Traverse through the compilation tables
        // of the definitions from the left most name
        // to the right most
        CompilationTable* ct = processing;
        bool static_context = false;
        std::string type;
        while (!traceback.empty()) {
            name = traceback.top();
            traceback.pop();

            // For a NormalMethodInvoke - Everything but the last thing is a field
            if (traceback.empty() && isMethod) {
                ClassMethod* cm = ct->getAClassMethod(methodSignature)->getClassMethod();

                // Static Context
                if (static_context) {
                    if (!cm->isStatic()) {
                        std::stringstream ss;
                        ss << "Cannot access non-static method '" << name->getNameId()->getIdAsString() << "' in a static context.";
                        NOTIFY_ERROR(name->getNameId()->getToken(), ss);
                    }

                    // Cannot chain static accesses.
                    static_context = false;
                // Non-static Context
                } else {
                    if (cm->isStatic()) {
                        std::stringstream ss;
                        ss << "Cannot access static method '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                        NOTIFY_ERROR(name->getNameId()->getToken(), ss);
                    }
                }

                continue;
            } else {
                // Can we find the declaration within this class?
                // If so, we can easily find the next CT to traverse to
                type = tryToGetTypename(name, ct);
                if (type == "") {
                    // Else, it is a class name or part of a class name
                    // These are also all static accesses
                    if (ct->getPackageName() == name->getFullName()) {
                        ct = ct;
                        static_context = true;
                        continue;
                    } else if (processing->checkTypePresenceFromSingleImport(name->getFullName())) {
                        ct = ct->checkTypePresenceFromSingleImport(name->getFullName());
                        static_context = true;
                        continue;
                    } else if (ct->checkTypePresenceInPackage(name->getFullName())) {
                        ct = ct->checkTypePresenceInPackage(name->getFullName());
                        static_context = true;
                        continue;
                    } else if (ct->checkTypePresenceFromImportOnDemand(name->getFullName(), name->getNameId()->getToken())) {
                        ct = ct->checkTypePresenceFromImportOnDemand(name->getFullName(), name->getNameId()->getToken());
                        static_context = true;
                        continue;
                    } else {
                        if (name->getNextName() != NULL) {
                            std::vector<CompilationTable*>& types = packages[name->getQualifier()];
                            for (unsigned int i = 0; i < types.size(); i++) {
                                if (types[i]->getCanonicalName() == name->getFullName()) {
                                    ct = types[i];
                                    static_context = true;
                                    break;
                                }
                            }
                        }

                        continue;
                    }
                }
            }

            // If it is not a findable field, it is a local decl/parameter
            // Which we always have access to
            if (ct->getAField(name->getNameId()->getIdAsString())) {
                FieldDecl* fd = ct->getAField(name->getNameId()->getIdAsString())->getField();

                // Static Context
                if (static_context) {
                    if (!fd->isStatic()) {
                        std::stringstream ss;
                        ss << "Cannot access non-static variable '" << name->getNameId()->getIdAsString() << "' in a static context.";
                        NOTIFY_ERROR(name->getNameId()->getToken(), ss);
                    }

                    // Cannot chain static accesses.
                    static_context = false;
                // Non-static Context
                } else {
                    if (fd->isStatic()) {
                        std::stringstream ss;
                        ss << "Cannot access static variable '" << name->getNameId()->getIdAsString() << "' in a non-static context.";
                        NOTIFY_ERROR(name->getNameId()->getToken(), ss);
                    }
                }
            }

            // Go to the next CT
            std::vector<CompilationTable*>& types = packages[getQualifierFromString(type)];
            for (unsigned int i = 0; i < types.size(); i++) {
                if (types[i]->getClassOrInterfaceName() == type.substr(type.find_last_of('.') + 1)) {
                    ct = types[i];
                    break;
                }
            }
        }
    }
}

// JLS 5.1.7
// Expects the full canonnical name of a type
bool TypeChecking::canAssignTypeToType(std::string lhs_string, std::string rhs_string) {
    last_type_to_type_error = GetTypeToTypeError(lhs_string, rhs_string);
    return last_type_to_type_error == "";
}

std::string TypeChecking::GetTypeToTypeError(std::string lhs_string, std::string rhs_string) {
    // JLS 5.1.4 - From the null type to any class type, interface type, or array type.
    if (!isPrimitive(lhs_string) && rhs_string == "null") {
        return "";
    }

    if (lhs_string == "void" || rhs_string == "void") {
        return "Cannot assign type void.";
    }

    // Narrowing Conversions must be explicit
    if (isPrimitive(lhs_string) && isPrimitive(rhs_string) && lhs_string != rhs_string &&
        !PrimitiveTypeConversions::isWideningConversion(lhs_string, rhs_string)) {
        return "Narrowing conversions must occur explicitly through casting.";
    }

    // There is no permitted conversion from any reference type to any primitive type.
    if (isPrimitive(lhs_string) && !isPrimitive(rhs_string)) {
        return "Cannot assign a reference type to a primitive type.";
    }

    // There is no permitted conversion from any primitive type to any reference type.
    if (isPrimitive(rhs_string) && !isPrimitive(lhs_string)) {
        return "Cannot assign a primitive type to a reference type.";
    }

    // There is no permitted conversion from the null type to any primitive type.
    if (rhs_string == "null" && isPrimitive(lhs_string)) {
        return "Cannot assign null to a primitive type.";
    }

    // There is no permitted conversion to the null type other than the identity conversion.
    if (lhs_string == "null" && rhs_string != "null") {
        return "Cannot assign anything to the type null.";
    }

    // There is no permitted conversion to the type boolean other than the identity conversion.
    if (lhs_string == "boolean" && rhs_string != "boolean") {
        return "Cannot assign a non-booleans to a boolean.";
    }

    // There is no permitted conversion from class type S to any array type if S is not Object.
    if (isPrimitive(rhs_string) && isArray(lhs_string)) {
        return "Cannot assign a primitive to an array.";
    }

    if (isArray(lhs_string) && !isArray(rhs_string)) {
        return "Cannot assign a non-array to an array.";
    }

    if (isArray(rhs_string) && !isArray(lhs_string)) {
        CompilationTable* lhs_ct = NULL;
        std::vector<CompilationTable*>& lhs_types = packages[getQualifierFromString(lhs_string)];
        for (unsigned int i = 0; i < lhs_types.size(); i++) {
            if (lhs_types[i]->getCanonicalName() == lhs_string) {
                lhs_ct = lhs_types[i];
            }
        }

        // There is no permitted conversion from any array type to any class type other than Object or String.
        if (lhs_ct->isClassSymbolTable() && lhs_string != "java.lang.Object" && lhs_string != "java.lang.String") {
            return "Cannot assign an array to a class other than Object or String.";
        }

        // There is no permitted conversion from any array type to any interface type, except to the
        // interface types java.io.Serializable and Cloneable, which are implemented by all arrays.
        if (!lhs_ct->isClassSymbolTable() && lhs_string != "java.io.Serializable" && lhs_string != "java.lang.Cloneable") {
            return "Array cannot be assigned to an interface type except for java.io.Serializable and java.lang.Cloneable.";
        }
    }

    if (!isPrimitive(lhs_string) && !isPrimitive(rhs_string) && !isArray(lhs_string) && !isArray(rhs_string)) {
        CompilationTable* lhs_ct = NULL;
        std::vector<CompilationTable*>& lhs_types = packages[getQualifierFromString(lhs_string)];
        for (unsigned int i = 0; i < lhs_types.size(); i++) {
            if (lhs_types[i]->getCanonicalName() == lhs_string) {
                lhs_ct = lhs_types[i];
            }
        }

        CompilationTable* rhs_ct = NULL;
        std::vector<CompilationTable*>& rhs_types = packages[getQualifierFromString(rhs_string)];
        for (unsigned int i = 0; i < rhs_types.size(); i++) {
            if (rhs_types[i]->getCanonicalName() == rhs_string) {
                rhs_ct = rhs_types[i];
            }
        }

        // There is no permitted conversion other than string conversion from
        // class type S to a different class type T if S is not a subclass of T
        if (rhs_ct->isClassSymbolTable() && lhs_ct->isClassSymbolTable() && !isSubclass(rhs_string, lhs_string)) {
            return "Cannot assign a class type to a different class type if neither is a subclass of the other.";
        }

        // There is no permitted conversion from class type S to interface
        // type K if S is final and does not implement K.
        if (!lhs_ct->isClassSymbolTable()) {
            Interfaces* interface;
            bool is_final;

            if (!rhs_ct->isClassSymbolTable()) {
                InterfaceTable* it = static_cast<InterfaceTable*>(rhs_ct->getSymbolTable());

                is_final = it->getInterface()->isFinal();
                interface = it->getInterface()->getExtendedInterfaces()->getListOfInterfaces();
            } else {
                ClassTable* ct = static_cast<ClassTable*>(rhs_ct->getSymbolTable());

                is_final = ct->getClass()->isFinal();
                interface = ct->getClass()->getImplementInterfaces()->getListOfInterfaces();
            }

            if (is_final) {
                bool implements = false;
                while (interface != NULL) {
                    if (interface->getCurrentInterface()->getFullName() == lhs_string) {
                        implements = true;
                        break;
                    }

                    interface = interface->getNextInterface();
                }

                if (!implements) {
                    return "Cannot assign a class type to an interface type if the class is final and does not implement the interface.";
                }
            }
        }

        // There is no permitted conversion other than string conversion from interface type
        // J to class type T if T is final and does not implement J.
        if (!rhs_ct->isClassSymbolTable()) {
            Interfaces* interface;
            bool is_final;

            if (!lhs_ct->isClassSymbolTable()) {
                InterfaceTable* it = static_cast<InterfaceTable*>(lhs_ct->getSymbolTable());

                is_final = it->getInterface()->isFinal();
                interface = it->getInterface()->getExtendedInterfaces()->getListOfInterfaces();
            } else {
                ClassTable* ct = static_cast<ClassTable*>(lhs_ct->getSymbolTable());

                is_final = ct->getClass()->isFinal();
                interface = ct->getClass()->getImplementInterfaces()->getListOfInterfaces();
            }

            if (is_final) {
                bool implements = false;
                while (interface != NULL) {
                    if (interface->getCurrentInterface()->getFullName() == rhs_string) {
                        implements = true;
                        break;
                    }

                    interface = interface->getNextInterface();
                }

                if (!implements) {
                    return "Cannot assign an interface type to a class type if the class is final and does not implement the interface.";
                }
            }
        }

        // There is no permitted conversion from interface type J to interface type K if J
        // and K contain methods with the same signature but different return types.
        if (!rhs_ct->isClassSymbolTable() && !lhs_ct->isClassSymbolTable()) {
            InterfaceTable* lhs_it = static_cast<InterfaceTable*>(lhs_ct->getSymbolTable());
            InterfaceTable* rhs_it = static_cast<InterfaceTable*>(rhs_ct->getSymbolTable());
            InterfaceMethod* lhs_im = lhs_it->getInterface()->getInterfaceBodyStar()->getInterfaceMethods();
            InterfaceMethod* rhs_im = rhs_it->getInterface()->getInterfaceBodyStar()->getInterfaceMethods();

            while (lhs_im != NULL) {
                InterfaceMethod* rhs_im_copy = rhs_im;

                while (rhs_im_copy != NULL) {
                    if (lhs_im->methodSignatureAsString() == rhs_im_copy->methodSignatureAsString() &&
                        lhs_im->getReturnType()->getTypeAsString() != rhs_im_copy->getReturnType()->getTypeAsString()) {
                        return "Cannot assign an interface type to an interface type that contains the same method signatures but different return types.";
                    }

                    rhs_im_copy = rhs_im_copy->getNextInterfaceMethod();
                }

                lhs_im = lhs_im->getNextInterfaceMethod();
            }
        }
    }

    // There is no permitted conversion from array type SC[] to array type TC[]
    // if there is no permitted conversion other than a string conversion from SC to TC.
    if (isArray(lhs_string) && isArray(rhs_string)) {
        rhs_string.erase(rhs_string.end() - 2, rhs_string.end());
        lhs_string.erase(lhs_string.end() - 2, lhs_string.end());

        // JLS 5.2 - Array assignment is not permitted if TC and SC are not the same primitive type
        if (isPrimitive(lhs_string) && isPrimitive(rhs_string) && rhs_string != lhs_string) {
            return "Cannot assign a primitive array type to a different primitive array type.";
        }

        return GetTypeToTypeError(lhs_string, rhs_string);
    }

    return "";
}

bool TypeChecking::isSubclass(std::string subclass, std::string superclass) {
    std::queue<CompilationTable*> traverse;
    std::vector<CompilationTable*>& types = packages[getQualifierFromString(subclass)];
    for (unsigned int i = 0; i < types.size(); i++) {
        if (types[i]->getCanonicalName() == subclass) {
            traverse.push(types[i]);
            break;
        }
    }

    CompilationTable* current_table;
    while (!traverse.empty()) {
        current_table = traverse.front();
        traverse.pop();

        if (current_table->getCanonicalName() == superclass) {
            return true;
        }

        SymbolTable* st = current_table->getSymbolTable();
        if (current_table->isClassSymbolTable()) {
            ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();

            if (cd->getSuper()->isImplicitlyExtending() ||
                !cd->noSuperClass()) {
                traverse.push(cd->getSuper()->getSuperClassTable());
            }
        }
    }

    return false;
}

void TypeChecking::findLocalDecls(SymbolTable* st, std::vector<LocalDecl*> &lds) {
    if (st == NULL) {
        return;
    }

    if (st->isClassMethodTable()) {
        findLocalDecls(static_cast<ClassMethodTable*>(st)->getSymbolTableOfMethod(), lds);
    } else if (st->isConstructorTable()) {
        findLocalDecls(static_cast<ConstructorTable*>(st)->getSymbolTableOfConstructor(), lds);
    } else if (st->isNestedBlockTable()) {
        findLocalDecls(static_cast<NestedBlockTable*>(st)->getSymbolTableOfBlock(), lds);
        findLocalDecls(static_cast<NestedBlockTable*>(st)->getNextTable(), lds);
    } else if (st->isForTable()) {
        findLocalDecls(static_cast<ForTable*>(st)->getForInitTable(), lds);
        findLocalDecls(static_cast<ForTable*>(st)->getLoopTable(), lds);
        findLocalDecls(static_cast<ForTable*>(st)->getNextTable(), lds);
    } else if (st->isLocalTable()) {
        lds.push_back(static_cast<LocalTable*>(st)->getLocalDecl());
        findLocalDecls(static_cast<LocalTable*>(st)->getNextTable(), lds);
    }
}

std::string TypeChecking::tryToGetTypename(Name* name, CompilationTable* cur_table) {
    // Check Local Declarations
    std::vector<LocalDecl*> lds;
    switch (cur_st_type) {
        case CLASSMETHOD_TABLE:
        case CONSTRUCTOR_TABLE:
            findLocalDecls(st_stack.top(), lds);
            break;
        case FIELDDECL_TABLE:
            break;
        default:
            assert(false);
    }

    for (unsigned int i = 0; i < lds.size(); i++) {
        if (lds[i]->getLocalId()->getIdAsString() == name->getNameId()->getIdAsString()) {
            return lds[i]->getLocalType()->getTypeAsString();
        }
    }

    // Check Method Parameters
    ParamList* pl;
    switch (cur_st_type) {
        case CLASSMETHOD_TABLE:
            pl = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader()
                                                               ->getClassMethodParams()->getListOfParameters();
            break;
        case CONSTRUCTOR_TABLE:{
            Constructor* c = static_cast<ConstructorTable*>(st_stack.top())->getConstructor();
            pl = c->getConstructorParameters()->getListOfParameters();
            break;
        }
        case FIELDDECL_TABLE:{
            pl = NULL;
            break;
        }
        default:
            assert(false);
    }

    while (pl != NULL) {
        if (pl->getParameterId()->getIdAsString() == name->getNameId()->getIdAsString()) {
            return pl->getParameterType()->getTypeAsString();
        }

        pl = pl->getNextParameter();
    }

    // Check Class Field Declarations
    if (cur_table->getAField(name->getNameId()->getIdAsString())) {
        return cur_table->getAField(name->getNameId()->getIdAsString())->getField()->getFieldType()->getTypeAsString();
    }

    return "";
}

bool TypeChecking::localDeclOrParam(Name* name, CompilationTable* cur_table) {
    // Check Local Declarations
    std::vector<LocalDecl*> lds;
    switch (cur_st_type) {
        case CLASSMETHOD_TABLE:
        case CONSTRUCTOR_TABLE:
            findLocalDecls(st_stack.top(), lds);
            break;
        case FIELDDECL_TABLE:
            break;
        default:
            assert(false);
    }

    for (unsigned int i = 0; i < lds.size(); i++) {
        if (lds[i]->getLocalId()->getIdAsString() == name->getNameId()->getIdAsString()) {
            return true;
        }
    }

    // Check Method Parameters
    ParamList* pl;
    switch (cur_st_type) {
        case CLASSMETHOD_TABLE:
            pl = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader()
                                                               ->getClassMethodParams()->getListOfParameters();
            break;
        case CONSTRUCTOR_TABLE:{
            Constructor* c = static_cast<ConstructorTable*>(st_stack.top())->getConstructor();
            pl = c->getConstructorParameters()->getListOfParameters();
            break;
        }
        case FIELDDECL_TABLE:{
            pl = NULL;
            break;
        }
        default:
            assert(false);
    }

    while (pl != NULL) {
        if (pl->getParameterId()->getIdAsString() == name->getNameId()->getIdAsString()) {
            return true;
        }

        pl = pl->getNextParameter();
    }

    return false;
}

bool TypeChecking::inheritsOrExtendsOrImplements(std::string classname, std::string searchname) {
    std::queue<CompilationTable*> traverse;
    std::vector<CompilationTable*>& types = packages[getQualifierFromString(classname)];
    for (unsigned int i = 0; i < types.size(); i++) {
        if (types[i]->getClassOrInterfaceName() == classname.substr(classname.find_last_of('.') + 1)) {
            traverse.push(types[i]);
            break;
        }
    }

    CompilationTable* current_table;
    while (!traverse.empty()) {
        current_table = traverse.front();
        traverse.pop();

        if (current_table->getCanonicalName() == searchname) {
            return true;
        }

        SymbolTable* st = current_table->getSymbolTable();
        if (current_table->isClassSymbolTable()) {
            ClassDecl* cd = static_cast<ClassTable*>(st)->getClass();

            if (!cd->noImplementedInterfaces()) {
                Interfaces* il = cd->getImplementInterfaces()->getListOfInterfaces();

                while (il != NULL) {
                    traverse.push(il->getImplOrExtInterfaceTable());
                    il = il->getNextInterface();
                }
            }

            if (!cd->noSuperClass()) {
                traverse.push(cd->getSuper()->getSuperClassTable());
            }

            if (cd->getSuper()->isImplicitlyExtending()) {
                traverse.push(cd->getSuper()->getSuperClassTable());
            }
        } else if (st) {
            InterfaceDecl* id = static_cast<InterfaceTable*>(st)->getInterface();

            if (!id->noExtendedInterfaces()) {
                Interfaces* il = id->getExtendedInterfaces()->getListOfInterfaces();

                while (il != NULL) {
                    traverse.push(il->getImplOrExtInterfaceTable());
                    il = il->getNextInterface();
                }
            }
        }
    }

    return false;
}
