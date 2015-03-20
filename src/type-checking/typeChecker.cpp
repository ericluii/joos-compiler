#include "typeChecker.h"
#include "primitiveTypeConversions.h"
#include "error.h"
#include <queue>
#include <sstream>
#include <stack>
#include <cassert>

/************************************************************************************

    THIS CODE IS UNHOLY. DO NOT TRY TO READ IT, OR BELIEVE THAT IT WAS WRITTEN
    WITH ANY GOOD CODING PRINCIPLES IN MIND. ALSO IF YOU WANT TO TRY FIGURING OUT
    WHAT IS GOING ON. GOOD LUCK. YOU HAVE BEEN WARNED.

                             ud$$$**$$$$$$$bc.                          
                          u@**"        4$$$$$$$Nu                       
                        J                ""#$$$$$$r                     
                       @                       $$$$b                    
                     .F                        ^*3$$$                   
                    :% 4                         J$$$N                  
                    $  :F                       :$$$$$                  
                   4F  9                       J$$$$$$$                 
                   4$   k             4$$$$bed$$$$$$$$$                 
                   $$r  'F            $$$$$$$$$$$$$$$$$r                
                   $$$   b.           $$$$$$$$$$$$$$$$$N                
                   $$$$$k 3eeed$$b    $$$Euec."$$$$$$$$$                
    .@$**N.        $$$$$" $$$$$$F'L $$$$$$$$$$$  $$$$$$$                
    :$$L  'L       $$$$$ 4$$$$$$  * $$$$$$$$$$F  $$$$$$F         edNc   
   @$$$$N  ^k      $$$$$  3$$$$*%   $F4$$$$$$$   $$$$$"        d"  z$N  
   $$$$$$   ^k     '$$$"   #$$$F   .$  $$$$$c.u@$$$          J"  @$$$$r 
   $$$$$$$b   *u    ^$L            $$  $$$$$$$$$$$$u@       $$  d$$$$$$ 
    ^$$$$$$.    "NL   "N. z@*     $$$  $$$$$$$$$$$$$P      $P  d$$$$$$$ 
       ^"*$$$$b   '*L   9$E      4$$$  d$$$$$$$$$$$"     d*   J$$$$$r   
            ^$$$$u  '$.  $$$L     "#" d$$$$$$".@$$    .@$"  z$$$$*"     
              ^$$$$. ^$N.3$$$       4u$$$$$$$ 4$$$  u$*" z$$$"          
                '*$$$$$$$$ *$b      J$$$$$$$b u$$P $"  d$$P             
                   #$$$$$$ 4$ 3*$"$*$ $"$'c@@$$$$ .u@$$$P               
                     "$$$$  ""F~$ $uNr$$$^&J$$$$F $$$$#                 
                       "$$    "$$$bd$.$W$$$$$$$$F $$"                   
                         ?k         ?$$$$$$$$$$$F'*                     
                          9$$bL     z$$$$$$$$$$$F                       
                           $$$$    $$$$$$$$$$$$$                        
                            '#$$c  '$$$$$$$$$"                          
                             .@"#$$$$$$$$$$$$b                          
                           z*      $$$$$$$$$$$$N.                       
                         e"      z$$"  #$$$k  '*$$.                     
                     .u*      u@$P"      '#$$c   "$$c                   
              u@$*"""       d$$"            "$$$u  ^*$$b.               
            :$F           J$P"                ^$$$c   '"$$$$$$bL        
           d$$  ..      @$#                      #$$b         '#$       
           9$$$$$$b   4$$                          ^$$k         '$      
            "$$6""$b u$$                             '$    d$$$$$P      
              '$F $$$$$"                              ^b  ^$$$$b$       
               '$W$$$$"                                'b@$$$$"         
                                                        ^$$$*  Gilo95'
*/

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
    numeric_expression_only(false),
    boolean_expression_only(false)
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
    }

    return true;
}

bool TypeChecking::check(FieldDecl* fieldDecl) {
    if (fieldDecl->isInitialized()) {
        std::string lefths = fieldDecl->getFieldType()->getTypeAsString();
        std::string righths = fieldDecl->getInitializingExpression()->getExpressionTypeString();
        if ((isPrimitive(lefths) && lefths != righths) ||
             (isArray(lefths) && lefths != righths) ||
             (!isPrimitive(lefths) && isPrimitive(righths)) ||
             (isPrimitive(lefths) && !isPrimitive(righths)) ||
             (!isPrimitive(lefths) && !isPrimitive(righths) &&
              (lefths != righths &&
               !inheritsOrExtendsOrImplements(righths, lefths) &&
               fieldDecl->getInitializingExpression()->getExprType() != ET_NULL))) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Bad Field Decl Init - RIGHTHS type is not LEFTHS type.");
            return false;
        }

        CHECK_PUSH_AND_SET(fieldDecl->getInitializingExpression(), fieldDecl->getFieldTable(), FIELDDECL_TABLE,
                           static_context_only, fieldDecl->isStatic());
    }

    return true;
}

bool TypeChecking::check(ClassMethod* classMethod) {
    CHECK_PUSH(classMethod->getMethodBody(), classMethod->getClassMethodTable(), CLASSMETHOD_TABLE);
}

bool TypeChecking::check(Constructor* constructor) {
    if (!constructor->emptyConstructorBody()) {
        CHECK_PUSH(constructor->getConstructorBody(), constructor->getConstructorTable(), CONSTRUCTOR_TABLE);
    } else {
        // Check against impilicit super() call
        ClassDecl* cd = static_cast<ClassTable*>(st_stack.top())->getClass();

        if (!cd->noSuperClass()) {
            CompilationTable* ct = cd->getSuper()->getSuperClassTable();
            if (!ct->getAConstructor("()")) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] No default constructor in super class.");
                return false;
            }
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

    return true;
}

bool TypeChecking::check(IfStmt* ifStmt) {
    bool else_result = true;
    if (!ifStmt->noElsePart()) {
        else_result = check(ifStmt->getExecuteFalsePart());
    }

    bool if_result = true;
    boolean_expression_only = true;
    if_result = check(ifStmt->getExpressionToEvaluate());
    boolean_expression_only = false;

    return if_result && check(ifStmt->getExecuteTruePart()) && else_result;
}

bool TypeChecking::check(WhileStmt* whileStmt) {
    boolean_expression_only = true;
    bool evaluate_result = check(whileStmt->getExpressionToEvaluate());
    boolean_expression_only = false;

    return evaluate_result && check(whileStmt->getLoopStmt());
}

bool TypeChecking::check(ForStmt* forStmt) {
    bool init_result = true;
    bool expression_result = true;
    bool update_result = true;

    if (!forStmt->emptyForInit()) { init_result = check(forStmt->getForInit()); }
    if (!forStmt->emptyExpression()) {
        boolean_expression_only = true;
        expression_result = check(forStmt->getExpressionToEvaluate());
        boolean_expression_only = false;
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
                bool ambiguous_traceback_needed = false;
                std::stack<Name*> ambiguousTraceback;
                if (tryToGetTypename(nextName, processing) == "") {
                    Name* tempName = nextName;
                    while (!tempName->isLastPrefix()) {
                        ambiguousTraceback.push(tempName);
                        tempName = tempName->getNextName();
                    }

                    if (tempName->getFullName() == processing->getClassOrInterfaceName()) {
                        ct = processing;
                        ambiguous_traceback_needed = !ambiguousTraceback.empty();
                    }

                    if (!ct) {
                        if (processing->getPackageName() == nextName->getFullName()) {
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
                }

                // Check if method invoke is static
                if (ct != NULL) {
                    CompilationTable* temp_processing = ct;
                    bool traceback_needed = !traceback.empty() || ambiguous_traceback_needed;
                    std::stack<Name*>* traceback_ptr = ambiguous_traceback_needed ? &ambiguousTraceback : &traceback;
                    while (!traceback_ptr->empty()) {
                        if (!temp_processing->getAField(traceback_ptr->top()->getNameId()->getIdAsString())) {
                            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Field does not exist.");
                            return false;
                        }

                        FieldDecl* fd = temp_processing->getAField(traceback_ptr->top()->getNameId()->getIdAsString())->getField();

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
                        traceback_ptr->pop();
                    }
                    ct = temp_processing;
                    ClassMethodTable* cmt = ct->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
                    ct = cmt->getDeclaringClass();
                    cmt = ct->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
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
                    } else if (cmt->getClassMethod()->isProtected() &&
                               (processing->getPackageName() != ct->getPackageName() &&
                                !inheritsOrExtendsOrImplements(processing->getCanonicalName(), ct->getCanonicalName()))) {
                        std::stringstream ss;
                        ss << "Protected method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                           << "' cannot be invoked here. - mh1";
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
                ct = cmt->getDeclaringClass();
                cmt = ct->getAClassMethod(static_cast<MethodNormalInvoke*>(methodInvoke)->methodInvocationMatchToSignature());
                if (cmt->getClassMethod()->isStatic()) {
                    std::stringstream ss;
                    ss << "Static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                       << "' cannot be invoked non-statically.";
                    Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                    return false;
                } else if (cmt->getClassMethod()->isProtected() &&
                           (processing->getPackageName() != ct->getPackageName() &&
                            !inheritsOrExtendsOrImplements(processing->getCanonicalName(), ct->getCanonicalName()))) {
                    std::stringstream ss;
                    ss << "Protected method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                       << "' cannot be invoked here. - mh3";
                    Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                    return false;
                }

                return true;                
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
                } else if (restrict_this) {
                    std::stringstream ss;
                    ss << "Non-Static method '" << cmt->getClassMethod()->getMethodHeader()->methodSignatureAsString()
                       << "' cannot be invoked in a static context.";
                    Error(E_TYPECHECKING, name->getNameId()->getToken(), ss.str());
                    return false;
                }
            }
        }
        return true;
    } else {
        return check(static_cast<InvokeAccessedMethod*>(methodInvoke)->getAccessedMethod());
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
        } else if (table->getAConstructor(newClassCreation->constructorInvocationMatchToSignature())->getConstructor()->isProtected()) {
            if (table->getPackageName() != processing->getPackageName()) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Constructor called is protected.");
                return false;
            }
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

bool TypeChecking::check(ReturnStmt* returnStmt) {
    if (returnStmt->isEmptyReturn()) {
        // Check that there is no return type?
        return true;
    } else {
        if (cur_st_type == CONSTRUCTOR_TABLE) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Constructor is returning not void.");
            return false;
        }

        MethodHeader* mh = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader();
        bool void_return = mh->isVoidReturnType();
        ExpressionStar* es = returnStmt->getReturnExpr();
        if (es->isEpsilon()) {
            if (void_return) {
                return true;
            } else {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Return type is void but returning stuff.");
                return false;
            }
        } else if (void_return) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Return type is not void but not returning stuff.");
            return false;
        } else if ((es->getExpression()->getExpressionTypeString() != mh->getReturnType()->getTypeAsString() &&
                    !inheritsOrExtendsOrImplements(es->getExpression()->getExpressionTypeString(), mh->getReturnType()->getTypeAsString()))&&
                   (!es->getExpression()->isExprTypeNull() && !isPrimitive(mh->getReturnType()->getTypeAsString()))) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Mismatching return types.");
            return false;
        }

        return check(es->getExpression());
    }
}

bool TypeChecking::check(Expression* expression) {
    if (numeric_expression_only &&
        (expression->getExprType() != ET_INT && expression->getExprType() != ET_SHORT &&
         expression->getExprType() != ET_BYTE && expression->getExprType() != ET_CHAR)) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Array access with non numeric.");
        return false;
    } else if (boolean_expression_only && expression->getExprType() != ET_BOOLEAN) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Condition is not a boolean.");
        return false;
    } else {
        boolean_expression_only = false;
        numeric_expression_only = false;
    }

    if(expression->isRegularBinaryExpression()) {
        // Something more than just NULL is happening
        restrict_num = false;

        Expression* leftExpr = static_cast<BinaryExpression*>(expression)->getLeftExpression();
        Expression* rightExpr = static_cast<BinaryExpression*>(expression)->getRightExpression();

        if (expression->isEqual() &&
            ((isPrimitive(leftExpr->getExpressionTypeString()) && !isPrimitive(rightExpr->getExpressionTypeString())) ||
             (!isPrimitive(leftExpr->getExpressionTypeString()) && isPrimitive(rightExpr->getExpressionTypeString())) ||
             (!isPrimitive(leftExpr->getExpressionTypeString()) && !isPrimitive(rightExpr->getExpressionTypeString()) &&
              leftExpr->getExpressionTypeString() != rightExpr->getExpressionTypeString() &&
              leftExpr->getExprType() != ET_NULL && rightExpr->getExprType() != ET_NULL &&
              !inheritsOrExtendsOrImplements(rightExpr->getExpressionTypeString(), leftExpr->getExpressionTypeString()) &&
              !inheritsOrExtendsOrImplements(leftExpr->getExpressionTypeString(), rightExpr->getExpressionTypeString())))) {
            //TODO : HOLY CRAP this obviously doesn't cover all failure cases
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Primitive to non-primitive comparison.");
            return false;
        }

        if (leftExpr->getExprType() == ET_VOID || rightExpr->getExprType() == ET_VOID) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Expressions with VOID");
            return false;
        }

        if((leftExpr->getExprType() == ET_INT || rightExpr->getExprType() == ET_INT) &&
           (expression->isEagerOr() || expression->isEagerAnd())) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Use of | or &.");
            return false;
        }
        
        if(expression->isMinus()){
            if(!isNumeric(leftExpr->getExpressionTypeString()) || !isNumeric(rightExpr->getExpressionTypeString())){
                //TODO
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Non numeric type used in binary minus");
                return false;
            }
        }

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

    if (!isLocalOrArg(name) && name->isSimpleName() &&
        processing->getAField(name->getFullName())) {
        FieldDecl* fd = processing->getAField(name->getFullName())->getField();

        if ((static_context_only || restrict_this) && !fd->isStatic()) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] non-static variable cannot be referenced in a static context.");
            return false;
        }
    } else if (!isLocalOrArg(name) && name->isSimpleName() &&
               restrict_type_name_expressions) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Illegal start type bracket type thing.");
        return false;
    } else if (!name->isSimpleName()) {
        Name* nextName = name->getNextName();
        CompilationTable* ct = NULL;
        std::string typeName = tryToGetTypename(nextName, processing);

        if (isLocalOrArg(nextName)) {
            if (typeName == "" || isPrimitive(typeName) ||
                (isArray(typeName) && name->getNameId()->getIdAsString() == "length")) {
                // uh.. TODO
                return true;
            } else if (isArray(typeName)) {
                typeName = "java.util.Arrays";
            }
            std::vector<CompilationTable*>& types = packages[getQualifierFromString(typeName)];
            for (unsigned int i = 0; i < types.size(); i++) {
                if (types[i]->getClassOrInterfaceName() == typeName.substr(typeName.find_last_of('.') + 1)) {
                    ct = types[i];
                    break;
                }
            }

            FieldDecl* fd = ct->getAField(name->getNameId()->getIdAsString())->getDeclaringClass()->getAField(name->getNameId()->getIdAsString())->getField();
            if ((static_context_only || restrict_this) && fd->isStatic()) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Cannot reference static variable from this instance.");
                return false;
            } else if (fd->isProtected() && processing->getPackageName() != ct->getPackageName()) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Field access to protected thing.");
                return false;
            }

            return true;
        } else if (processing->getPackageName() == nextName->getFullName()) {
            ct = processing;
        } else if (processing->checkTypePresenceFromSingleImport(nextName->getFullName())) {
            ct = processing->checkTypePresenceFromSingleImport(nextName->getFullName());
        } else if (processing->checkTypePresenceInPackage(nextName->getFullName())) {
            ct = processing->checkTypePresenceInPackage(nextName->getFullName());
        } else if (processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken())) {
            ct = processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken());
        } else {
            if (restrict_type_name_expressions) {
                std::vector<CompilationTable*>& types = packages[getQualifierFromString(name->getFullName())];
                for (unsigned int i = 0; i < types.size(); i++) {
                    if (types[i]->getCanonicalName() == name->getFullName()) {
                        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Illegal start type bracket type thing.");
                        return false;
                    }
                }
            }

            std::vector<CompilationTable*>& types = packages[getQualifierFromString(typeName)];
            for (unsigned int i = 0; i < types.size(); i++) {
                if (types[i]->getClassOrInterfaceName() == typeName.substr(typeName.find_last_of('.') + 1)) {
                    ct = types[i];
                    break;
                }
            }
        }

        if ((static_context_only || restrict_this) && ct && !ct->getAField(name->getNameId()->getIdAsString())->getField()->isStatic()) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] non-static variable cannot be referenced in a static context.");
            return false;
        } else if (ct && ct->getAField(name->getNameId()->getIdAsString())->getField()->isProtected() &&
                   (processing->getPackageName() != ct->getPackageName() &&
                    !inheritsOrExtendsOrImplements(processing->getCanonicalName(), ct->getCanonicalName()))) {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Name expression, protected access bleh.");
            return false;
        }
    }

    return true;
}

bool TypeChecking::check(InstanceOf* instanceOf) {
    if(isPrimitive(instanceOf->getTypeToCheck()->getTypeAsString())){
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot use non-reference types in an instanceof.");
        return false;
    }
    if(!(instanceOf->getExpressionToCheck()->isExprTypeNull() || 
         instanceOf->getExpressionToCheck()->isExprTypeObject() ||
         instanceOf->getExpressionToCheck()->isExprTypeObjectArray() ||
         isPrimitiveArray(instanceOf->getExpressionToCheck()->getExpressionTypeString()))){
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot use non-reference, non-null types in an instanceof.");
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

    return true;
}

bool TypeChecking::check(PrimaryNewArray* primaryNewArray) {
    numeric_expression_only = true;
    bool rv = check(primaryNewArray->getTheDimension());
    numeric_expression_only = false;
    return rv;
}

bool TypeChecking::check(ArrayAccess* arrayAccess) {
    numeric_expression_only = true;
    bool rv = check(arrayAccess->getAccessExpression());
    numeric_expression_only = false;

    if (arrayAccess->isArrayAccessPrimary()) {
        return check(static_cast<ArrayAccessPrimary*>(arrayAccess)->getAccessedPrimaryArray()) && rv;
    }

    return rv;
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
    Expression* paramaterExpression = castExpression->getExpressionToCast();
    //std::cout << "Casting: " << paramaterExpression->getExpressionTypeString();
    if(castExpression->isCastToArrayName()){
        return true;
    } else if(castExpression->isCastToReferenceType()){
        //std::cout << " to Reference type" << std::endl;
        if(isPrimitive(paramaterExpression->getExpressionTypeString()) || isPrimitiveArray(paramaterExpression->getExpressionTypeString()))
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast primitive to reference type");
        }
        else
        {
            if(!inheritsOrExtendsOrImplements(paramaterExpression->getExpressionTypeString(), castExpression->getExpressionTypeString()) &&
               !inheritsOrExtendsOrImplements(castExpression->getExpressionTypeString(), paramaterExpression->getExpressionTypeString()) &&
               paramaterExpression->getExprType() != ET_NULL)
            {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] invalid reference type cast");
            }
        }
    } else if(castExpression->isCastToPrimitiveType()){
        //std::cout << " to Primitive type" << std::endl;
        if(!(isPrimitive(paramaterExpression->getExpressionTypeString()) || isPrimitiveArray(paramaterExpression->getExpressionTypeString())))
        {
            //std::cout << "Error on casting " << paramaterExpression->getExpressionTypeString() << " to Primitive type" << std::endl;
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast reference type to primitive type");
        }
        else if(paramaterExpression->isExprTypeBoolean() && (castExpression->getExprType() != ET_BOOLEAN))
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a boolean");
        }
        else if(isPrimitive(paramaterExpression->getExpressionTypeString()) && static_cast<CastPrimitive*>(castExpression)->isPrimitiveArrayCast())
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a primitive to a primitive array");
        }
        else if(isPrimitiveArray(paramaterExpression->getExpressionTypeString()) && !static_cast<CastPrimitive*>(castExpression)->isPrimitiveArrayCast())
        {
            Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] cannot cast a primitive array to a primitive");
        }
    } else {
        assert(false);
    }
    return true;
}

bool TypeChecking::check(LiteralOrThis* literalOrThis) {
    if (literalOrThis->isThis() && restrict_this) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] this in field init or static method.");
        return false;
    } else if (literalOrThis->isNumber() && restrict_num) {
        Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Number after '!'.");
        return false;
    }

    return true;
}

bool TypeChecking::check(Assignment* assignment) {
    bool rv = true;
    if (assignment->isAssignName()) {
        Name* name = static_cast<AssignName*>(assignment)->getNameToAssign();
        
        if (name->isSimpleName()) {

        } else {
            Name* nextName = name->getNextName();
            std::string typeName = tryToGetTypename(nextName, processing);
            if ((isArray(typeName) && name->getNameId()->getIdAsString() == "length")) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Assigning to final field length.");
                return false;
            }

            CompilationTable* ct = NULL;
            if (typeName == processing->getCanonicalName()) {
                ct = processing;
            } else if (typeName != "") {
                std::vector<CompilationTable*>& types = packages[getQualifierFromString(typeName)];
                for (unsigned int i = 0; i < types.size(); i++) {
                    if (types[i]->getClassOrInterfaceName() == typeName.substr(typeName.find_last_of('.') + 1)) {
                        ct = types[i];
                        break;
                    }
                }
            } else if (processing->getCanonicalName() == nextName->getFullName()) {
                ct = processing;
            } else if (processing->checkTypePresenceFromSingleImport(nextName->getFullName())) {
                ct = processing->checkTypePresenceFromSingleImport(nextName->getFullName());
            } else if (processing->checkTypePresenceInPackage(nextName->getFullName())) {
                ct = processing->checkTypePresenceInPackage(nextName->getFullName());
            } else if (processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken())) {
                ct = processing->checkTypePresenceFromImportOnDemand(nextName->getFullName(), nextName->getNameId()->getToken());
            } else {
                typeName = nextName->getFullName();
                std::vector<CompilationTable*>& types = packages[getQualifierFromString(typeName)];
                for (unsigned int i = 0; i < types.size(); i++) {
                    if (types[i]->getClassOrInterfaceName() == typeName.substr(typeName.find_last_of('.') + 1)) {
                        ct = types[i];
                        break;
                    }
                }
            }

            ct = ct->getAField(name->getNameId()->getIdAsString())->getDeclaringClass();
            FieldDecl* fd = ct->getAField(name->getNameId()->getIdAsString())->getField();
            if (fd->isProtected() &&
                (processing->getPackageName() != ct->getPackageName() &&
                 !inheritsOrExtendsOrImplements(processing->getCanonicalName(), ct->getCanonicalName()))) {
                Error(E_DEFAULT, NULL, "[DEV NOTE - REPLACE] Field access to protected thing - assignment.");
                return false;
            }
        }
    } else if (assignment->isAssignField()) {
        rv = check(static_cast<AssignField*>(assignment)->getAssignedField());
    }

    std::string lhs_type = assignment->getExpressionTypeString();
    if (assignmentCheck(lhs_type, assignment->getExpressionToAssign())) {
        return check(assignment->getExpressionToAssign()) && rv;
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
        return check(localDecl->getLocalInitExpr());
    }

    std::stringstream ss;
    ss << "Declaration of '" << localDecl->getLocalId()->getIdAsString() << "' of type '"
       << localDecl->getLocalType()->getTypeAsString()
       << "' cannot be assigned as '" << localDecl->getLocalInitExpr()->getExpressionTypeString() << ".'";

    Error(E_TYPECHECKING, localDecl->getLocalId()->getToken(), ss.str());
    return false;
}

bool TypeChecking::isLocalOrArg(Name* name) {
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
                return true;
            }
        }
        bs = bs->getNextBlockStmt();
    }

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
            return true;
        }
        pl = pl->getNextParameter();
    }

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
            if ((!isPrimitive(lefths) && !isArray(lefths)) &&
                ((lefths == "java.lang.Object" || lefths == "java.lang.Cloneable") ||
                 (righths == lefths) ||
                 (!isArray(lefths) && inheritsOrExtendsOrImplements(righths, lefths)))) {
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
