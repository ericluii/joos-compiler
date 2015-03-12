#include <sstream>
#include <iostream>
#include "typeLinker.h"
#include "compilationUnit.h"
#include "error.h"
#include "classDecl.h"
#include "interfaceDecl.h"
#include "super.h"
#include "interfaceList.h"
#include "interfaces.h"
#include "classBodyStar.h"
#include "fieldDecl.h"
#include "classMethod.h"
#include "constructor.h"
#include "referenceType.h"
#include "binaryExpression.h"
#include "instanceof.h"
#include "negationExpression.h"
#include "assignField.h"
#include "assignArray.h"
#include "assignName.h"
#include "arrayAccessPrimary.h"
#include "bracketedExpression.h"
#include "invokeAccessedMethod.h"
#include "newClassCreation.h"
#include "primaryNewArray.h"
#include "qualifiedThis.h"
#include "primaryExpression.h"
#include "castName.h"
#include "methodHeader.h"
#include "formalParamStar.h"
#include "paramList.h"
#include "methodBody.h"
#include "blockStmtsStar.h"
#include "localDecl.h"
#include "returnStmt.h"
#include "expressionStar.h"
#include "stmtExprAssign.h"
#include "stmtExprInvoke.h"
#include "stmtExprCreation.h"
#include "nestedBlock.h"
#include "ifStmt.h"
#include "whileStmt.h"
#include "forStmt.h"
#include "interfaceBodyStar.h"
#include "interfaceMethod.h"
#include "argumentsStar.h"
#include "arguments.h"

TypeLinker::TypeLinker(std::map<std::string, std::vector<CompilationTable*> >& packages) : packages(packages) {}

TypeLinker::~TypeLinker() {}

void TypeLinker::checkSingleImportClashError(const std::string& typeName, const std::string& packageName,
                                             ImportDecls* imports) {
    std::string importType;
    std::string importQualifier;
    Token* importTok;
    if(imports->isSingleImport()) {
        importTok = imports->getCurrentImport()->getNameId()->getToken();
        importQualifier = imports->getCurrentImport()->getQualifier();
        importType = importTok->getString();
        if(importType == typeName && importQualifier != packageName) {
            // if the import type clashes with the type defined in this file
            // and it's not an import of the type in this file
            std::stringstream ss;
            ss << "Single type import '" << imports->getCurrentImport()->getFullName()
               << "' clashes with type '" << typeName << "'.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    }
}

void TypeLinker::checkImportsClashWithType(const std::string& typeName, const std::string& packageName,
                                           ImportDecls* imports) {
    checkSingleImportClashError(typeName, packageName, imports);

    while(!imports->isLastImport()) {
        imports = imports->getNextImport();
        checkSingleImportClashError(typeName, packageName, imports);
    }
}

void TypeLinker::checkForClashingSingleImportInFile(CompilationTable* compilation) {
    std::string classOrInterfaceName = compilation->getClassOrInterfaceName();
    if(classOrInterfaceName != "") {
        // indicates that a type was defined
        CompilationUnit* unit = compilation->getCompilationUnit();
        ImportDeclsStar* imports = unit->getImportDeclsStar();
        if(!imports->isEpsilon()) {
            checkImportsClashWithType(classOrInterfaceName, compilation->getPackageName(), imports->getImportDeclarations());
        }
    }
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

void TypeLinker::setImportIfExist(CompilationTable* compilation, ImportDecls* import) {
    std::stringstream ss;
    Name* importName = import->getCurrentImport();
    Token* importTok = importName->getNameId()->getToken();
    std::string packageName  = importName->getQualifier();
    std::string typeName = importTok->getString();
  
    if(import->isSingleImport()) {
        if(packageName != "") {
            if(packages.count(packageName) == 1) {
                bool typeExist = false;
                for(unsigned int i = 0; i < packages[packageName].size(); i++) {
                    if(packages[packageName][i]->getClassOrInterfaceName() == typeName) {
                        compilation->setASingleTypeImport(typeName, packages[packageName][i], importTok);
                        typeExist = true;
                    }
                }

                if(!typeExist) {
                    // the type wasn't found in the package
                    ss << "Single type import '" << packageName << "." << typeName
                       << "' specified an undefined type.";
                    Error(E_TYPELINKING, importTok, ss.str());
                }
            } else {
                // package does not exist
                ss << "Single type import specified package '" << packageName
                   << "' which does not exist among the given files in the command line.";
                Error(E_TYPELINKING, importName->getNextName()->getNameId()->getToken(), ss.str());
            }
        } else {
            // package named is default package, which is not possible
            ss << "Single type import '" << typeName << "' didn't specify a package.";
            Error(E_TYPELINKING, importTok, ss.str());
        }
    } else {
        // import on demand
        std::string fullPackageName;
        if(packageName != "") {
            fullPackageName = packageName + '.';
        }
        fullPackageName+= typeName;

        if(packages.count(fullPackageName) == 1) {
            compilation->setAnImportTypeOnDemand(fullPackageName, &packages[fullPackageName]);
        } else {
            bool prefixPackage = false;
            // this is to avoid prefixes like jav in java.lang, which would be invalid
            std::string prefixPackageName = fullPackageName + '.';
            std::map<std::string, std::vector<CompilationTable*> >::iterator it;
            for(it = packages.begin(); it != packages.end(); it++) {
                if(it->first != "") {
                    // if not the default package
                    if(it->first.find(prefixPackageName) == 0) {
                        // if the package name is actually a prefix of some package name
                        // the package won't really have any compilation table associated with it
                        prefixPackage = true;
                    }
                }
            }

            if(!prefixPackage) {
                // if the package name is not a prefix of any package name
                // specified in the given files
                ss << "Import type on demand '" << fullPackageName
                   << ".*' did not specify an existing package or a prefix of some existing package.";
                Error(E_TYPELINKING, importTok, ss.str());
            }
        }
    }
}

void TypeLinker::importsExistenceRecurse(CompilationTable* compilation, ImportDecls* import) {
    if(!import->isLastImport()) {
        importsExistenceRecurse(compilation, import->getNextImport());
    }

    setImportIfExist(compilation, import);
}

void TypeLinker::checkImportsExistenceAndSet(CompilationTable* compilation) {
    ImportDeclsStar* imports = compilation->getCompilationUnit()->getImportDeclsStar();
    if(!imports->isEpsilon()) {
        importsExistenceRecurse(compilation, imports->getImportDeclarations());
    }

    // by default all compilation unit imports java.lang as an import type on demand
    compilation->setAnImportTypeOnDemand("java.lang", &packages["java.lang"]);
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

bool TypeLinker::checkIfNameConflictsWithType(CompilationTable* compilation, Name* name, bool isType = false) {
    if(name == NULL) {
        return false;
    }
    
    std::string qualifier = name->getQualifier();
    std::string potentialTypeName = name->getNameId()->getIdAsString();
    if(qualifier != "") {
        // if not the default package
        if(packages.count(qualifier) == 1) {
            std::vector<CompilationTable*>::iterator it;
            for(it = packages[qualifier].begin(); it != packages[qualifier].end(); it++) {
                if((*it)->getClassOrInterfaceName() == potentialTypeName) {
                    // indicates that the package name is actually a type name
                    return true;
                }
            }
        }
    } else {
        // if the qualifier's the default package
        if(isType) {
            // if the Name being checked right now resolved to a type, then check in the
            // environment i.e in single imports, in packaged and in import type on demand
            if((compilation->checkTypePresenceFromSingleImport(potentialTypeName) != NULL) ||
               (compilation->checkTypePresenceInPackage(potentialTypeName) != NULL) ||
               (compilation->checkTypePresenceFromImportOnDemand(potentialTypeName, name->getNameId()->getToken()) != NULL)) {
                return true;
            }
        }
    }

    return checkIfNameConflictsWithType(compilation, name->getNextName(), isType);
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------


void TypeLinker::checkImportsResolveToTypes(CompilationTable* compilation, ImportDecls* import) {
    if(!import->isLastImport()) {
        checkImportsResolveToTypes(compilation, import->getNextImport());
    }
   
    std::stringstream ss;
    if(import->isSingleImport()) {
        bool conflict = false;
        if(checkIfNameConflictsWithType(compilation, import->getCurrentImport()->getNextName())) {
            conflict = true;
        } else {
            // if it doesn't conflict as with the above check
            if(compilation->getPackageName() != "") {
                // do this check ONLY IF the current compilation is not in the default package
                Name* name = import->getCurrentImport();
                while(!name->isLastPrefix()) {
                    name = name->getNextName();
                }
                std::string potentialTypeName = name->getNameId()->getIdAsString();
                if((compilation->checkTypePresenceFromSingleImport(potentialTypeName) != NULL) ||
                   (compilation->checkTypePresenceInPackage(potentialTypeName) != NULL) ||
                   (compilation->checkTypePresenceFromImportOnDemand(potentialTypeName, name->getNameId()->getToken()) != NULL)) {
                    conflict = true;
                }
            }
        }

        if(conflict) {
            // if there was a conflict
            ss << "Single type import '" << import->getCurrentImport()->getFullName()
               << "' or a prefix of it resolves to a type not in the default package.";
            Error(E_TYPELINKING, import->getCurrentImport()->getNameId()->getToken(), ss.str());
        }
    } else {
        // import type on demand
        if(checkIfNameConflictsWithType(compilation, import->getCurrentImport())) {
            ss << "Import on demand '" << import->getCurrentImport()->getFullName()
               << "' or a prefix of it resolves to a type not in the default package.";
            Error(E_TYPELINKING, import->getCurrentImport()->getNameId()->getToken(), ss.str());
        }
    }
}

void TypeLinker::checkPackageResolveToTypes(CompilationTable* compilation, PackageDecl* package) {
    if(checkIfNameConflictsWithType(compilation, package->getPackageName())) {
        std::stringstream ss;
        ss << "Package name '" << compilation->getPackageName()
           << "' or a prefix of it resolves to a type not in the default package.";
        Error(E_TYPELINKING, package->getPackageName()->getNameId()->getToken(), ss.str());
    }
}

void TypeLinker::checkPackageAndImportsResolveToTypes(CompilationTable* compilation) {
    CompilationUnit* unit = compilation->getCompilationUnit();
    PackageDecl* package = unit->getPackageDecl();
    ImportDeclsStar* imports = unit->getImportDeclsStar();

    checkPackageResolveToTypes(compilation, package);
    if(!imports->isEpsilon()) {
        checkImportsResolveToTypes(compilation, imports->getImportDeclarations());
    }
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

void TypeLinker::linkTypeNames(CompilationTable* compilation, CompilationUnit* unit) {
    if(!unit->noTypeDeclared()) {
        // if the compilation unit has a defined type
        if(unit->getTypeDecl()->isClass()) {
            linkTypeNames(compilation, (ClassDecl*) unit->getTypeDecl());
        } else {
            linkTypeNames(compilation, (InterfaceDecl*) unit->getTypeDecl());
        }
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ClassDecl* type) {
    std::string canonicalName = compilation->getCanonicalName();
    if(!type->noSuperClass()) { 
        if(canonicalName != "java.lang.Object") {
            linkTypeNames(compilation, type->getSuper());
        } else {
            std::stringstream ss;
            ss << "Class java.lang.Object may not have a super class.";
            Error(E_TYPELINKING, type->getClassId()->getToken(), ss.str()); 
        }
    } else {
        if(canonicalName != "java.lang.Object") {
            // if it's not java.lang.Object itself and it does not have a superclass
            // then make java.lang.Object it's superclass
            std::vector<CompilationTable*>& types = packages["java.lang"];
            for(unsigned int i = 0; i < types.size(); i++) {
                if(types[i]->getClassOrInterfaceName() == "Object") {
                    type->getSuper()->setSuperClassTable(types[i]);
                    type->getSuper()->setImplicitExtend();
                    break;
                }
            }
        }
    }

    if(!type->noImplementedInterfaces()) { linkTypeNames(compilation, type->getImplementInterfaces()); }
    if(!type->emptyBody()) { linkTypeNames(compilation, type->getClassMembers()); }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Super* super) {
    CompilationTable* linkType = linkTypeNames(compilation, super->getSuperName());
    if(linkType == NULL) {
        // type cannot be found
        reportTypeNameLinkError("Extending class '", super->getSuperName()->getFullName(),
                                super->getSuperName()->getNameId()->getToken());
    }

    super->setSuperClassTable(linkType);
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, InterfaceList* interfaces) {
    if(!interfaces->isEpsilon()) {
        linkTypeNames(compilation, interfaces->getListOfInterfaces());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Interfaces* interface) {
    if(!interface->isLastInterface()) {
        linkTypeNames(compilation, interface->getNextInterface());
    }

    CompilationTable* linkType = linkTypeNames(compilation, interface->getCurrentInterface());
    if(linkType == NULL) {
        // type cannot be found
        reportTypeNameLinkError("Implementing or extending interface '", interface->getCurrentInterface()->getFullName(),
                                interface->getCurrentInterface()->getNameId()->getToken());
    }
    interface->setImplOrExtInterfaceTable(linkType);
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ClassBodyStar* classBody) {
    linkTypeNames(compilation, classBody->getBody());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ClassBodyDecls* classMember) {
    if(!classMember->isLastClassMember()) {
        linkTypeNames(compilation, classMember->getNextDeclaration());
    }

    if(classMember->isField()) {
        linkTypeNames(compilation, (FieldDecl*) classMember);
    } else if(classMember->isClassMethod()) {
        linkTypeNames(compilation, (ClassMethod*) classMember);
    } else {
        // constructor
        linkTypeNames(compilation, (Constructor*) classMember);
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, FieldDecl* field) {
    linkTypeNames(compilation, field->getFieldType());
    if(field->isInitialized()) {
        linkTypeNames(compilation, field->getInitializingExpression());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Type* type) {
    if(type->isClassOrInterfaceType() || type->isReferenceArrayType()) {
        Name* typeName = ((ReferenceType*) type)->getReferenceName();
        CompilationTable* linkType = linkTypeNames(compilation, typeName);
        if(linkType == NULL) { 
            reportTypeNameLinkError("Specifying type '", typeName->getFullName(), typeName->getNameId()->getToken());
        }

        ((ReferenceType*) type)->setReferenceTypeTable(linkType);
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Expression* expr) {
    if(expr->isRegularBinaryExpression()) {
        linkTypeNames(compilation, ((BinaryExpression*) expr)->getLeftExpression());
        linkTypeNames(compilation, ((BinaryExpression*) expr)->getRightExpression());
    } else if(expr->isInstanceOf()) {
        linkTypeNames(compilation, ((InstanceOf*) expr)->getExpressionToCheck());
        linkTypeNames(compilation, ((InstanceOf*) expr)->getTypeToCheck());
    } else if(expr->isNumericNegation() || expr->isBooleanNegation()) {
        linkTypeNames(compilation, ((NegationExpression*) expr)->getNegatedExpression());
    } else if(expr->isAssignField() || expr->isAssignArray() || expr->isAssignName()) {
        linkTypeNames(compilation, (Assignment*) expr);
    } else if(expr->isPrimaryExpression()) {
        linkTypeNames(compilation, (PrimaryExpression*) expr);
    } else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
        linkTypeNames(compilation, (CastExpression*) expr);
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Assignment* assign) {
    if(assign->isAssignField()) {
        linkTypeNames(compilation, ((AssignField*) assign)->getAssignedField());
    } else if(assign->isAssignArray()) {
        linkTypeNames(compilation, ((AssignArray*) assign)->getAssignedArray());
    } else {
        // assign name
        linkTypeNames(compilation, ((AssignName*) assign)->getNameToAssign());
    }

    linkTypeNames(compilation, assign->getExpressionToAssign());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, FieldAccess* fieldAccessed) {
    linkTypeNames(compilation, fieldAccessed->getAccessedFieldPrimary());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ArrayAccess* array) {
    if(array->isArrayAccessPrimary()) {
        // only when it is an array access primary
        linkTypeNames(compilation, ((ArrayAccessPrimary*) array)->getAccessedPrimaryArray());
    }
    linkTypeNames(compilation, array->getAccessExpression());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, PrimaryExpression* primExpr) {
    linkTypeNames(compilation, primExpr->getPrimaryExpression());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Primary* prim) {
    if(prim->isArrayAccessPrimary() || prim->isArrayAccessName()) {
        linkTypeNames(compilation, (ArrayAccess*) prim);
    } else if(prim->isBracketedExpression()) {
        linkTypeNames(compilation, ((BracketedExpression*) prim)->getExpressionInside());
    } else if(prim->isNormalMethodCall() || prim->isAccessedMethodCall()) {
        linkTypeNames(compilation, (MethodInvoke*) prim);
    } else if(prim->isNewClassCreation()) {
        linkTypeNames(compilation, (NewClassCreation*) prim);
    } else if(prim->isNewReferenceArray()) {
        // only really when it is a an array of some class or interface
        linkTypeNames(compilation, ((PrimaryNewArray*) prim)->getArrayType());
    } else if(prim->isFieldAccess()) {
        linkTypeNames(compilation, (FieldAccess*) prim);
    } else if(prim->isQualifiedThis()) {
        QualifiedThis* qualThis = (QualifiedThis*) prim;
        CompilationTable* linkType = linkTypeNames(compilation, qualThis->getQualifyingClassName());
        if(linkType == NULL) {
            reportTypeNameLinkError("Qualifying this expression with class '", qualThis->getQualifyingClassName()->getFullName(),
                                    qualThis->getQualifyingClassName()->getNameId()->getToken());
        } else {
            // found the class
            if(linkType->getCanonicalName() == compilation->getCanonicalName()) {
                qualThis->setQualifyingClassTable(linkType);
            } else {
                // it is not the current class
                std::stringstream ss;
                ss << "Qualifying class '" << qualThis->getQualifyingClassName()
                   << "' for this expression is not allowed since it is not the class the expression is in.";
                Error(E_TYPELINKING, qualThis->getQualifyingClassName()->getNameId()->getToken(), ss.str());
            }
        }
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, MethodInvoke* invoke) {
    if(invoke->isAccessedMethodCall()) {
        linkTypeNames(compilation, ((InvokeAccessedMethod*) invoke)->getAccessedMethod());
    }
    linkTypeNames(compilation, invoke->getArgsForInvokedMethod());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ArgumentsStar* args) {
    if(!args->isEpsilon()) {
        linkTypeNames(compilation, args->getListOfArguments());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Arguments* arg) {
    if(!arg->lastArgument()) {
        linkTypeNames(compilation, arg->getNextArgs());
    }

    linkTypeNames(compilation, arg->getSelfArgumentExpr());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, NewClassCreation* create) {
    CompilationTable* linkType = linkTypeNames(compilation, create->getClassName());
    if(linkType == NULL) {
        reportTypeNameLinkError("Creating class '", create->getClassName()->getFullName(),
                                create->getClassName()->getNameId()->getToken());
        return;
    }

    if(linkType->aTypeWasDefined() && !linkType->isClassSymbolTable()) {
        std::stringstream ss;
        ss << "Cannot create interface '" << create->getClassName()->getFullName() << "' through a class instance creation expression.";
        Error(E_TYPELINKING, create->getClassName()->getNameId()->getToken(), ss.str());
    }
    create->setTableOfCreatedClass(linkType);

    // attempt to link arguments
    linkTypeNames(compilation, create->getArgsToCreateClass());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, CastExpression* castExpr) {
    if(castExpr->isCastToArrayName() || castExpr->isCastToReferenceType()) {
        // if it's a cast to some class/interface or an array of class/interface
        CastName* castName = (CastName*) castExpr;
        Name* castTo = castName->getNameToCastTo();
        CompilationTable* linkType = NULL;
        linkType = linkTypeNames(compilation, castTo);
        if(linkType == NULL) {
            reportTypeNameLinkError("Casting to '", castName->getTypeToCastAsString(),
                                    castTo->getNameId()->getToken());
            return;
        }

        castName->setCastTypeTable(linkType);
    }

    linkTypeNames(compilation, castExpr->getExpressionToCast());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ClassMethod* method) {
    linkTypeNames(compilation, method->getMethodHeader());
    linkTypeNames(compilation, method->getMethodBody());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, MethodHeader* header) {
    if(!header->isVoidReturnType()) {
        linkTypeNames(compilation, header->getReturnType());
    }
    linkTypeNames(compilation, header->getClassMethodParams());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, FormalParamStar* params) {
    if(!params->isEpsilon()) {
        linkTypeNames(compilation, params->getListOfParameters());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ParamList* params) {
    if(!params->isLastParameter()) {
        linkTypeNames(compilation, params->getNextParameter());
    }

    linkTypeNames(compilation, params->getParameterType());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, MethodBody* body) {
    if(!body->noDefinition()) {
        // only if it actually has a body, even if it's empty
        linkTypeNames(compilation, body->getBlockStmtsStar());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, BlockStmtsStar* stmts) {
    if(!stmts->isEpsilon()) {
        linkTypeNames(compilation, stmts->getStatements());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, BlockStmts* stmts) {
    if(!stmts->isLastStatement()) {
        linkTypeNames(compilation, stmts->getNextBlockStmt());
    }
    if(stmts->isLocalVarDecl()) {
        linkTypeNames(compilation, ((LocalDecl*) stmts)->getLocalType());
        linkTypeNames(compilation, ((LocalDecl*) stmts)->getLocalInitExpr());
    } else if(stmts->isReturnStmt()) {
        linkTypeNames(compilation, ((ReturnStmt*) stmts)->getReturnExpr());
    } else if(stmts->isAssignStmt() || stmts->isClassCreationStmt() || stmts->isMethodInvokeStmt()) {
        linkTypeNames(compilation, (StmtExpr*) stmts);
    } else if(stmts->isNestedBlock()) {
        linkTypeNames(compilation, ((NestedBlock*) stmts)->getNestedBlock());
    } else if(stmts->isIfStmt() || stmts->isIfThenElseStmt()) {
        linkTypeNames(compilation, (IfStmt*) stmts);
    } else if(stmts->isWhileStmt()) {
        linkTypeNames(compilation, (WhileStmt*) stmts);
    } else if(stmts->isForStmt()) {
        linkTypeNames(compilation, (ForStmt*) stmts);
    } 
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ExpressionStar* exprStar) {
    if(!exprStar->isEpsilon()) {
        linkTypeNames(compilation, exprStar->getExpression());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, StmtExpr* stmtExpr) {
    if(stmtExpr->isAssignStmt()) {
        linkTypeNames(compilation, ((StmtExprAssign*) stmtExpr)->getAssignmentExpression());
    } else if(stmtExpr->isMethodInvokeStmt()) {
        linkTypeNames(compilation, ((StmtExprInvoke*) stmtExpr)->getMethodInvoked());
    } else {
        // class creation stmt
        linkTypeNames(compilation, ((StmtExprCreation*) stmtExpr)->getCreatedClass());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, IfStmt* stmt) {
    linkTypeNames(compilation, stmt->getExpressionToEvaluate());
    linkTypeNames(compilation, stmt->getExecuteTruePart());
    if(!stmt->noElsePart()) {
        linkTypeNames(compilation, stmt->getExecuteFalsePart());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, WhileStmt* stmt) {
    linkTypeNames(compilation, stmt->getExpressionToEvaluate());
    linkTypeNames(compilation, stmt->getLoopStmt());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, ForStmt* stmt) {
    if(!stmt->emptyForInit()) {
        linkTypeNames(compilation, stmt->getForInit());
    }
    linkTypeNames(compilation, stmt->getExpressionToEvaluate());
    if(!stmt->emptyForUpdate()) {
        linkTypeNames(compilation, stmt->getForUpdate());
    }
    linkTypeNames(compilation, stmt->getLoopStmt());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, Constructor* ctor) {
    linkTypeNames(compilation, ctor->getConstructorParameters());
    linkTypeNames(compilation, ctor->getConstructorBody());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, InterfaceDecl* type) {
    linkTypeNames(compilation, type->getExtendedInterfaces());
    linkTypeNames(compilation, type->getInterfaceBodyStar());
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, InterfaceBodyStar* body) {
    if(!body->isEpsilon()) {
        linkTypeNames(compilation, body->getInterfaceMethods());
    }
}

void TypeLinker::linkTypeNames(CompilationTable* compilation, InterfaceMethod* methods) {
    if(!methods->isLastMethod()) {
        linkTypeNames(compilation, methods->getNextInterfaceMethod());
    }

    if(!methods->isVoidReturnType()) {
        linkTypeNames(compilation, methods->getReturnType());
    }
    linkTypeNames(compilation, methods->getParametersList());
}

CompilationTable* TypeLinker::linkTypeNames(CompilationTable* compilation, Name* name) {
    std::string typeName = name->getNameId()->getIdAsString();
    CompilationTable* linkType = NULL;
    if(name->isQualifiedName()) {
        std::string qualifier = name->getQualifier();
        if(qualifier != "" && packages.count(qualifier) == 1) {
            // if it can be found in the packages which are not default packages
            bool typeExist = false;
            unsigned int indexExist = 0;
            for(unsigned int i = 0; i != packages[qualifier].size(); i++) {
                if(packages[qualifier][i]->getClassOrInterfaceName() == typeName) {
                    indexExist = i;
                    typeExist = true;
                    break;
                }
            }

            if(!typeExist) { return NULL; }
            if(checkIfNameConflictsWithType(compilation, name->getNextName(), true)) {
                std::stringstream ss;
                ss << "Fully qualified name '" << name->getFullName()
                   << "' resolves to a type, but it's strict prefix is also a type itself in this compilation unit's environment.";
                Error(E_TYPELINKING, name->getNameId()->getToken(), ss.str());
            }
            linkType = packages[qualifier][indexExist];
        }
    } else {
        // simple name
        linkType = compilation->checkTypePresenceFromSingleImport(typeName);
        if(linkType != NULL) { return linkType; }
        linkType = compilation->checkTypePresenceInPackage(typeName);
        if(linkType != NULL) { return linkType; }
        linkType = compilation->checkTypePresenceFromImportOnDemand(typeName, name->getNameId()->getToken());
    }

    return linkType;
}
// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

void TypeLinker::reportTypeNameLinkError(const std::string& errorMsg, const std::string& typeName, Token* tok) {
    std::stringstream ss;
    ss << errorMsg << typeName 
        << "' is not possible because type does not exist within the other compilation units/single type import/package/import type on demand.";
    Error(E_TYPELINKING, tok, ss.str());
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

void TypeLinker::typeLinkingResolution() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = packages.begin(); it != packages.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            checkForClashingSingleImportInFile(*it2);
            if (Error::count() > 0) { return; }
            checkImportsExistenceAndSet(*it2);
            if (Error::count() > 0) { return; }
            checkPackageAndImportsResolveToTypes(*it2);
            if (Error::count() > 0) { return; }
            linkTypeNames(*it2, (*it2)->getCompilationUnit());
            if (Error::count() > 0) { return; }
        }
    }
}
