#include "typeChecker.h"
#include "error.h"
#include <sstream>
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

TypeChecking::TypeChecking(std::map<std::string, std::vector<CompilationTable*> >& packages) :
    packages(packages) {}

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
    SymbolTable* st = compilation->getSymbolTable();

    if (st == NULL || !st->isClassTable()) {
        return true;
    }

    return check(static_cast<ClassTable*>(st)->getClass());
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
    return check(classMethod->getMethodBody());
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
    } else {
        return rest_of_statements;
    }
}

bool TypeChecking::check(LocalDecl* localDecl) {
    std::string type = localDecl->getLocalType()->getTypeAsString();

    switch (localDecl->getLocalInitExpr()->getExprType()) {
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
            if (isPrimitive(type) || isPrimitiveArray(type)) {
                if (localDecl->getLocalInitExpr()->getExpressionTypeString() == type) {
                    return true;
                }
            } else if (isArray(localDecl->getLocalInitExpr()->getExpressionTypeString())) {
                // Arrays extend Objects who implement Cloneable
                if (type == "java.lang.Cloneable") {
                    return true;
                }
                // Arrays implicitly implement serializable
                if (type == "java.io.Serializable") {
                    return true;
                }
                // Arrays extend Objects
                if (type == "java.lang.Object") {
                    return true;
                }
                
            }

            break;
        }
        case ET_NULL:
             if (!isPrimitive(type)) {
                return true;
             }

             break;
        case ET_VOID:
             break;
        case ET_OBJECT:
            if (!isPrimitive(type) && !isArray(type)) {
                return true;
            }

            break;
        case ET_OBJECTARRAY:
            if (!isPrimitive(type) && !isPrimitiveArray(type)) {
                return true;
            }

            break;
        case ET_NOTEVALUATED:
        default:
            assert(false);
    };

    std::stringstream ss;
    ss << "Declaration of '" << localDecl->getLocalId()->getIdAsString() << "' of type '" << type
       << "' cannot be assigned as '" << localDecl->getLocalInitExpr()->getExpressionTypeString() << ".'";

    Error(E_TYPECHECKING, localDecl->getLocalId()->getToken(), ss.str());
    return false;
}
