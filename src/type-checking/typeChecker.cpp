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
    manager(manager), packages(packages) {}

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

    st_stack.push(st);
    bool ra = check(static_cast<ClassTable*>(st)->getClass());
    st_stack.pop();
    return ra;
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
        return check(static_cast<ClassMethod*>(classBodyDecls)) && rest_of_body;
    } else if (classBodyDecls->isConstructor()) {
        return check(static_cast<Constructor*>(classBodyDecls)) && rest_of_body;
    } else {
        return rest_of_body;
    }
}

bool TypeChecking::check(ClassMethod* classMethod) {
    st_stack.push(classMethod->getClassMethodTable());
    bool ra = check(classMethod->getMethodBody());
    st_stack.pop();
    return ra;
}

bool TypeChecking::check(Constructor* constructor) {
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
    } else {
        return rest_of_statements;
    }
}

bool TypeChecking::check(MethodInvoke* methodInvoke) {
    if (methodInvoke->isNormalMethodCall()) {
        Name* name = static_cast<MethodNormalInvoke*>(methodInvoke)->getNameOfInvokedMethod();
        
        if (!name->isSimpleName()) {
            Name* nextName = name->getNextName();
            CompilationTable* ct = NULL;
            std::stack<Name*> traceback;

            while (nextName != NULL && ct == NULL) {
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
                    std::string typeName;

                    // Look for class field declaration
                    // Check Local Declarations
                    MethodBody* cm = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodBody();
                    BlockStmts* bs = cm->getBlockStmtsStar()->getStatements();
                    while (bs != NULL) {
                        if (bs->isLocalVarDecl()) {
                            LocalDecl* ld = static_cast<LocalDecl*>(bs);
                            if (ld->getLocalId()->getIdAsString() == nextName->getFullName()) {
                                typeName = static_cast<LocalDecl*>(bs)->getLocalType()->getTypeAsString();
                                break;
                            }
                        }

                        bs = bs->getNextBlockStmt();
                    }

                    // Check Method Parameters
                    if (typeName == "") {
                        ParamList* pl = static_cast<ClassMethodTable*>(st_stack.top())->getClassMethod()->getMethodHeader()
                                                                                      ->getClassMethodParams()->getListOfParameters();

                        while (pl != NULL) {
                            if (pl->getParameterId()->getIdAsString() == nextName->getFullName()) {
                                typeName = pl->getParameterType()->getTypeAsString();
                                break;
                            }

                            pl = pl->getNextParameter();
                        }
                    }
                    
                    // Check Class Field Declarations
                    if (typeName == "" && processing->getAField(nextName->getFullName())) {
                        typeName = processing->getAField(nextName->getFullName())->getField()->getFieldType()->getTypeAsString();
                    }

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
        }
        return true;
    } else {
        //std::cout << "accessed: " << static_cast<InvokeAccessedMethod*>(methodInvoke)->methodInvocationMatchToSignature() << std::endl;
        return true;
    }
}

bool TypeChecking::check(NewClassCreation* stmtExprCreation) {
    // TODO
    return true;
}

bool TypeChecking::check(ReturnStmt* returnStmt) {
    // TODO
    return true;
}

bool TypeChecking::check(Assignment* assignment) {
    /*std::string lefths;
    Expression* righths = assignment->getExpressionToAssign();
    Token* token;

    if (assignment->isAssignName()) {
        AssignName* an = static_cast<AssignName*>(assignment);
        lefths = an->getNameToAssign()->getFullName();
        token = an->getNameToAssign()->getNameId()->getToken();
        std::cout << lefths << std::endl;
    } else if (assignment->isAssignArray()) {
        //TODO
        lefths = static_cast<AssignArray*>(assignment)->getAssignedArray()->getAccessExpression()->getExpressionTypeString();
        return true;
    } else {
        //TODO
        return true;
    }

    if (assignmentCheck(lefths, righths)) {
        return true;
    }

    std::stringstream ss;
    ss << "Assignment of '" << lefths << "' cannot be assigned as '" << righths->getExpressionTypeString() << ".'";
    Error(E_TYPECHECKING, token, ss.str());*/
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
