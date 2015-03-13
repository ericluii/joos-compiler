#include "typeChecker.h"
#include "primitiveTypeConversions.h"
#include "error.h"
#include <queue>
#include <sstream>
#include <stack>
#include <cassert>

bool isPrimitive(std::string type) {
    return type == "int" || type == "short" || type == "byte" || type == "char" || type == "boolean";
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
    restrict_null(false)
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
        rest_of_body = check(classBodyDecls->getNextDeclaration());
    }

    if (classBodyDecls->isClassMethod()) {
        restrict_this = classBodyDecls->isStatic();
        bool rv = check(static_cast<ClassMethod*>(classBodyDecls)) && rest_of_body;
        restrict_this = false;
        return rv;
    } else if (classBodyDecls->isConstructor()) {
        return check(static_cast<Constructor*>(classBodyDecls)) && rest_of_body;
    } else if (classBodyDecls->isField()) {
        restrict_this = classBodyDecls->isStatic();
        bool rv = check(static_cast<FieldDecl*>(classBodyDecls)) && rest_of_body;
        restrict_this = false;
        return rv;
    } else {
        return true;
    }
}

bool TypeChecking::check(FieldDecl* fieldDecl) {
    if (fieldDecl->isInitialized()) {
        //return check(fieldDecl->getInitializingExpression());
        CHECK_PUSH(fieldDecl->getInitializingExpression(), fieldDecl->getFieldTable(), FIELDDECL_TABLE);
    }

    return true;
}

bool TypeChecking::check(ClassMethod* classMethod) {
    CHECK_PUSH(classMethod->getMethodBody(), classMethod->getClassMethodTable(), CLASSMETHOD_TABLE);
}

bool TypeChecking::check(Constructor* constructor) {
    if (!constructor->emptyConstructorBody()) {
        CHECK_PUSH(constructor->getConstructorBody(), constructor->getConstructorTable(), CONSTRUCTOR_TABLE);
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

    return true;
}

bool TypeChecking::check(IfStmt* ifStmt) {
    bool else_result = true;
    if (!ifStmt->noElsePart()) {
        else_result = check(ifStmt->getExecuteFalsePart());
    }

    return check(ifStmt->getExpressionToEvaluate()) && check(ifStmt->getExecuteTruePart()) && else_result;
}

bool TypeChecking::check(WhileStmt* whileStmt) {
    restrict_null = true;
    bool evaluate_result = check(whileStmt->getExpressionToEvaluate());
    restrict_null = false;

    return evaluate_result && check(whileStmt->getLoopStmt());
}

bool TypeChecking::check(ForStmt* forStmt) {
    bool init_result = true;
    bool expression_result = true;
    bool update_result = true;

    if (!forStmt->emptyForInit()) { init_result = check(forStmt->getForInit()); }
    if (!forStmt->emptyExpression()) {
        restrict_null = true;
        expression_result = check(forStmt->getExpressionToEvaluate());
        restrict_null = false;
    }
    if (!forStmt->emptyForUpdate()) { update_result = check(forStmt->getForUpdate()); }

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
        
        if (!name->isSimpleName()) {
            Name* nextName = name->getNextName();
            CompilationTable* ct = NULL;
            std::stack<Name*> traceback;
            while (nextName != NULL && ct == NULL) {
                if (tryToGetTypename(nextName, processing) == "") {
                    if (processing->getCanonicalName() == nextName->getFullName()) {
                        ct = processing;
                    } else if (processing->checkTypePresenceFromSingleImport(nextName->getFullName())) {
                        ct = processing->checkTypePresenceFromSingleImport(nextName->getFullName());
                    } else if (processing->checkTypePresenceInPackage(nextName->getFullName())) {
                        ct = processing->checkTypePresenceInPackage(nextName->getFullName());
                    } else if (processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken())) {
                        ct = processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken());
                    } else {
                        std::vector<CompilationTable*>& types = packages[getQualifierFromString(nextName->getFullName())];
                        for (unsigned int i = 0; i < types.size(); i++) {
                            if (types[i]->getClassOrInterfaceName() == nextName->getFullName().substr(nextName->getFullName().find_last_of('.') + 1)) {
                                ct = types[i];
                                break;
                            }
                        }
                    }
                }

                // Check if method invoke is static
                if (ct != NULL) {
                    CompilationTable* temp_processing = ct;
                    bool traceback_needed = !traceback.empty();
                    while (!traceback.empty()) {
                        FieldDecl* fd = temp_processing->getAField(traceback.top()->getNameId()->getIdAsString())->getField();

                        // Only the first can be invoked statically
                        if (temp_processing == ct && !fd->isStatic()) {
                            std::stringstream ss;
                            ss << "Non-static field '" << fd->getFieldDeclared()->getIdAsString()
                               << "' cannot be accessed statically.";
                            Error(E_TYPECHECKING, fd->getFieldDeclared()->getToken(), ss.str());
                            return false;
                        } else if (temp_processing != ct && fd->isStatic()) {
                            std::stringstream ss;
                            ss << "Static field '" << fd->getFieldDeclared()->getIdAsString()
                               << "' cannot be accessed non-statically.";
                            Error(E_TYPECHECKING, fd->getFieldDeclared()->getToken(), ss.str());
                            return false;
                        }

                        std::string aType = fd->getFieldType()->getTypeAsString();
                        std::vector<CompilationTable*>& types = packages[getQualifierFromString(aType)];
                        for (unsigned int i = 0; i < types.size(); i++) {
                            if (types[i]->getClassOrInterfaceName() == aType.substr(aType.find_last_of('.') + 1)) {
                                temp_processing = types[i];
                                break;
                            }
                        }

                        assert(temp_processing != ct);
                        traceback.pop();
                    }
                    ct = temp_processing;
                    ClassMethodTable* cmt = ct->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
                    
                    if (!traceback_needed && !cmt->getClassMethod()->isStatic()) {
                        std::stringstream ss;
                        ss << "Non-static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                           << "' cannot be invoked statically.";
                        Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                        return false;
                    } else if (traceback_needed && cmt->getClassMethod()->isStatic()) {
                        std::stringstream ss;
                        ss << "Static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                           << "' cannot be invoked non-statically.";
                        Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                        return false;
                    }

                    return true;
                // Non-Static Invoke, Check to see if method is static
                } else {
                    std::string typeName = tryToGetTypename(nextName, processing);
                    if (typeName == "") {
                        traceback.push(nextName);
                        nextName = nextName->getNextName();
                        continue;
                    }

                    std::vector<CompilationTable*>& types = packages[getQualifierFromString(typeName)];
                    for (unsigned int i = 0; i < types.size(); i++) {
                        if (types[i]->getClassOrInterfaceName() == typeName.substr(typeName.find_last_of('.') + 1)) {
                            ct = types[i];
                            break;
                        }
                    }
                }

                assert(ct != NULL);

                ClassMethodTable* cmt = ct->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
                if (!cmt->getClassMethod()->isStatic()) {
                    return true;
                }

                std::stringstream ss;
                ss << "Static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                   << "' cannot be invoked non-statically.";
                Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                return false;
            }
        } else {
            // Simple name
            // Just need to check that the method being called is not static, if it exists in the class
            ClassMethodTable* cmt = processing->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
            if (cmt != NULL) {
                if (cmt->getClassMethod()->isStatic()) {
                    std::stringstream ss;
                    ss << "Static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                       << "' cannot be invoked non-statically.";
                    Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                    return false;
                }
            }
        }
        return true;
    } else {
        //std::cout << "accessed: " << static_cast<InvokeAccessedMethod*>(methodInvoke)->methodInvocationMatchToSignature() << std::endl;
        return true;
    }
}

bool TypeChecking::check(NewClassCreation* newClassCreation) {
    CompilationTable* table = newClassCreation->getTableOfCreatedClass();
    if (table->isClassSymbolTable()) {
        ClassTable* ct = static_cast<ClassTable*>(table->getSymbolTable());
        ClassDecl* cd = ct->getClass();

        if (cd->isAbstract()) {
            std::stringstream ss;
            ss << "Abstract class '" << cd->getClassId()->getIdAsString() << "' cannot be constructed.";
            Error(E_TYPECHECKING, newClassCreation->getClassName()->getNameId()->getToken(), ss.str());
            return false;
        }
    }

    return true;
}

bool TypeChecking::check(ReturnStmt* returnStmt) {
    if (returnStmt->isEmptyReturn()) {
        // Check that there is no return type?
        return true;
    } else {
        ExpressionStar* es = returnStmt->getReturnExpr();

        if (es->isEpsilon()) {
            return true;
        }

        return check(es->getExpression());
    }
}

bool TypeChecking::check(Expression* expression) {
    if(expression->isRegularBinaryExpression()) {
        // Something more than just NULL is happening
        restrict_null = false;

        Expression* leftExpr = static_cast<BinaryExpression*>(expression)->getLeftExpression();
        Expression* rightExpr = static_cast<BinaryExpression*>(expression)->getRightExpression();

        if (leftExpr->getExprType() == ET_VOID || rightExpr->getExprType() == ET_VOID) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Expressions with VOID");
            return false;
        }

        if((leftExpr->getExprType() == ET_INT || rightExpr->getExprType() == ET_INT) &&
           (expression->isEagerOr() || expression->isEagerAnd())) {
            //TODO
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Use of | or &.");
            return false;
        }

        return check(rightExpr) && check(leftExpr);
    } else if(expression->isAssignName() || expression->isAssignField() || expression->isAssignArray()) {
        return check(static_cast<Assignment*>(expression));
    /*} else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
        traverseAndLink((CastExpression*) expr);
    } else if(expr->isInstanceOf()) {

    } else if(expr->isNameExpression()) {
*/
    } else if(expression->isPrimaryExpression()) {
        return check(static_cast<PrimaryExpression*>(expression)->getPrimaryExpression());
    }

    return true;
}

bool TypeChecking::check(Primary* primary) {
   /* if(primary->isFieldAccess()) {
        traverseAndLink((FieldAccess*) prim);
    } else if(primary->isThis() || primary->isNumber() || primary->isTrueBoolean() || primary->isFalseBoolean()
              || primary->isCharLiteral() || primary->isStringLiteral() || primary->isNull()) {
        traverseAndLink((LiteralOrThis*) prim);
    } else if(prim->isBracketedExpression()) {
        traverseAndLink((BracketedExpression*) prim);
        // linking has been resolved, but BracketedExpression doesn't really link to anywhere
        // indicate this
        prim->ResolveLinkButNoEntity();
    } else if(prim->isNewClassCreation()) {
        traverseAndLink((NewClassCreation*) prim);
    } else if(prim->isNewPrimitiveArray() || prim->isNewReferenceArray()) {
        traverseAndLink((PrimaryNewArray*) prim);
    } else if(prim->isNormalMethodCall() || prim->isAccessedMethodCall()) {
        traverseAndLink((MethodInvoke*) prim);
    } else if(prim->isArrayAccessName() || prim->isArrayAccessPrimary()) {
        traverseAndLink((ArrayAccess*) prim);
    } else {
        // the remaining is qualified this, which has already been resolved
        // indicate this
        prim->ResolveLinkButNoEntity();
    }*/
    if (primary->isFieldAccess()) {
        return check(static_cast<FieldAccess*>(primary)->getAccessedFieldPrimary());
    } else if(primary->isThis() || primary->isNumber() || primary->isTrueBoolean() || primary->isFalseBoolean()
              || primary->isCharLiteral() || primary->isStringLiteral() || primary->isNull()) {
        return check(static_cast<LiteralOrThis*>(primary)); 
    } else if (primary->isBracketedExpression()) {
        return check(static_cast<BracketedExpression*>(primary)->getExpressionInside());
    } else if (primary->isNewClassCreation()) {
        return check(static_cast<NewClassCreation*>(primary));
    } else if(primary->isNormalMethodCall() || primary->isAccessedMethodCall()) {
        return check(static_cast<MethodInvoke*>(primary));
    }

    return true;
}

bool TypeChecking::check(LiteralOrThis* literalOrThis) {
    if (literalOrThis->isThis() && restrict_this) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] this in field init or static method.");
        return false;
    } else if (literalOrThis->isNull() && restrict_null) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] NULL in the loops.");
        return false;
    }

    return true;
}

bool TypeChecking::check(Assignment* assignment) {
    std::string lhs_type = assignment->getExpressionTypeString();
    if (assignmentCheck(lhs_type, assignment->getExpressionToAssign())) {
        return true;
    }
    
    // for now it isn't worth the effort to try to get the token...
    std::stringstream ss;
    ss << "Assignment of '" << "filler" << "' of type '"
       << lhs_type
       << "' cannot be assigned the type '" << assignment->getExpressionToAssign()->getExpressionTypeString() << ".'";
    Error(E_DEFAULT, NULL, ss.str());
    return false;
}

bool TypeChecking::check(LocalDecl* localDecl) {
    if (assignmentCheck(localDecl->getLocalType()->getTypeAsString(), localDecl->getLocalInitExpr())) {
        return true;
    }

    std::stringstream ss;
    ss << "Declaration of '" << localDecl->getLocalId()->getIdAsString() << "' of type '"
       << localDecl->getLocalType()->getTypeAsString()
       << "' cannot be assigned as '" << localDecl->getLocalInitExpr()->getExpressionTypeString() << ".'";

    Error(E_TYPECHECKING, localDecl->getLocalId()->getToken(), ss.str());
    return false;
}

std::string TypeChecking::tryToGetTypename(Name* name, CompilationTable* cur_table) {
    // Check Local Declarations
    BlockStmts* bs;
    switch (cur_st_type) {
        case CLASSMETHOD_TABLE:{
            MethodBody* cm = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodBody();
            bs = cm->getBlockStmtsStar()->getStatements();
            break;
        }
        case CONSTRUCTOR_TABLE:{
            Constructor* c = static_cast<ConstructorTable*>(st_stack.top())->getConstructor();
            bs = c->getConstructorBody()->getStatements();
            break;
        }
        case FIELDDECL_TABLE:{
            bs = NULL;
            break;
        }
        default:
            assert(false);
    }
    
    while (bs != NULL) {
        if (bs->isLocalVarDecl()) {
            LocalDecl* ld = static_cast<LocalDecl*>(bs);
            if (ld->getLocalId()->getIdAsString() == name->getFullName()) {
                return static_cast<LocalDecl*>(bs)->getLocalType()->getTypeAsString();
            }
        }

        bs = bs->getNextBlockStmt();
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
        if (pl->getParameterId()->getIdAsString() == name->getFullName()) {
            return pl->getParameterType()->getTypeAsString();
        }

        pl = pl->getNextParameter();
    }
    
    // Check Class Field Declarations
    if (cur_table->getAField(name->getFullName())) {
        return cur_table->getAField(name->getFullName())->getField()->getFieldType()->getTypeAsString();
    }

    return "";
}

bool TypeChecking::assignmentCheck(std::string lefths, Expression* expr) {
    std::string righths = expr->getExpressionTypeString();

    switch (expr->getExprType()) {
        case ET_INT:
        case ET_SHORT:
        case ET_BYTE:
        case ET_CHAR:
        case ET_BOOLEAN:
        case ET_INTARRAY:
        case ET_SHORTARRAY:
        case ET_BYTEARRAY:
        case ET_CHARARRAY:
        case ET_BOOLEANARRAY: {
            // Deal with primitive types first
            if (isPrimitive(lefths)){
                if(PrimitiveTypeConversions::isWideningConversion(lefths, righths) || lefths == righths){
                    return true;
                }    
            } else if(isPrimitiveArray(lefths)) {
                if (righths == lefths) {
                    return true;
                }
            } else if (isArray(righths)) {
                // Arrays extend Objects who implement Cloneable
                if (lefths == "java.lang.Cloneable") {
                    return true;
                }
                // Arrays implicitly implement serializable
                if (lefths == "java.io.Serializable") {
                    return true;
                }
                // Arrays extend Objects
                if (lefths == "java.lang.Object") {
                    return true;
                }
                
            }

            break;
        }
        case ET_NULL:
             if (!isPrimitive(lefths)) {
                return true;
             }

             break;
        case ET_VOID:
             break;
        case ET_OBJECT:
            if ((!isPrimitive(lefths) && !isArray(lefths)) ||
                (lefths == "java.lang.Object" || lefths == "java.lang.Cloneable") ||
                (righths == lefths) ||
                (!isArray(lefths) && inheritsOrExtendsOrImplements(righths, lefths))) {
                return true;
            }

            break;
        case ET_OBJECTARRAY:{
            if ((lefths == "java.lang.Object" || lefths == "java.lang.String") ||
                (righths == lefths)) {
                return true;
            }

            if (isArray(lefths)) {
                righths.erase(righths.end() - 2, righths.end());
                lefths.erase(lefths.end() - 2, lefths.end());

                if (inheritsOrExtendsOrImplements(righths, lefths)) {
                    return true;
                }

                lefths += "[]";
            }

            break;
        }
        case ET_NOTEVALUATED:
        default:
            assert(false);
    };

    return false;
}

bool TypeChecking::check(MethodInvoke* methodInvoke) {
    ClassMethod *method = methodInvoke->getReferredClassMethod()->getClassMethod();
    if(methodInvoke->isNormalMethodCall() && !method->isStatic()){
        return true;
    }
    if(methodInvoke->isAccessedMethodCall() && method->isStatic()){
        return true;
    }
    
    std::stringstream ss;
    ss << "Access of '" << method->getMethodHeader()->methodSignatureAsString() << "is illegal";

    Error(E_TYPECHECKING, method->getMethodHeader()->getClassMethodId()->getToken(), ss.str());
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
