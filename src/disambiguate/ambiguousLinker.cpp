#include <sstream>
#include <cassert>
#include "error.h"
#include "package.h"
#include "ambiguousLinkType.h"
#include "evaluatedType.h"
#include "ambiguousLinker.h"
#include "paramTable.h"
#include "classMethodTable.h"

#include "packagesManager.h"
#include "compilationTable.h"
#include "classTable.h"
#include "classDecl.h"
#include "classBodyStar.h"
#include "fieldDecl.h"
#include "localDecl.h"
#include "binaryExpression.h"
#include "assignName.h"
#include "name.h"
#include "referenceType.h"
#include "primitiveType.h"
#include "assignField.h"
#include "newClassCreation.h"
#include "literalOrThis.h"
#include "primaryNewArray.h"
#include "bracketedExpression.h"
#include "argumentsStar.h"
#include "arguments.h"
#include "paramList.h"
#include "methodNormalInvoke.h"
#include "invokeAccessedMethod.h"
#include "qualifiedThis.h"
#include "assignArray.h"
#include "arrayAccessName.h"
#include "arrayAccessPrimary.h"
#include "castName.h"
#include "castPrimitive.h"
#include "instanceof.h"
#include "nameExpression.h"
#include "primaryExpression.h"
#include "classMethod.h"

#include "interfaceDecl.h"

AmbiguousLinker::AmbiguousLinker(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& compilations) :
            manager(manager), compilations(compilations), curSymTable(NULL), curCompilation(NULL) {}

AmbiguousLinker::~AmbiguousLinker() {}

// ---------------------------------------------------------------------------
// Class part

void AmbiguousLinker::traverseAndLink(ClassDecl* aClass) {
    traverseAndLink(aClass->getClassMembers());
}

void AmbiguousLinker::traverseAndLink(ClassBodyStar* body) {
    if(!body->isEpsilon()) {
        traverseAndLink(body->getBody());
    }
}

void AmbiguousLinker::traverseAndLink(ClassBodyDecls* body) {
    if(!body->isLastClassMember()) {
        traverseAndLink(body->getNextDeclaration());
    }

    if(body->isField()) {
        traverseAndLink((FieldDecl*) body);
    } else if(body->isClassMethod()) {
    } else {
        // constructor
    }
}

void AmbiguousLinker::traverseAndLink(FieldDecl* field) {
    if(field->isInitialized()) {
        traverseAndLink(field->getInitializingExpression(), false);
    }
    // update the symbol table
    curSymTable = field->getFieldTable();
}

void AmbiguousLinker::traverseAndLink(Expression* expr, bool withinMethod) {
    if(expr->isRegularBinaryExpression()) {
        Expression* leftExpr = ((BinaryExpression*) expr)->getLeftExpression();
        Expression* rightExpr = ((BinaryExpression*) expr)->getRightExpression();
        traverseAndLink(leftExpr, withinMethod);
        traverseAndLink(rightExpr, withinMethod);
        if(expr->isLazyOr() || expr->isLazyAnd() || expr->isEagerOr() || expr->isEagerAnd() ||
           expr->isEqual() || expr->isNotEqual() || expr->isLT() || expr->isGT() || expr->isLTE() || expr->isGTE() ||
           expr->isInstanceOf()) {
            // specualate for now that expression type is boolean
            expr->setExprType(ET_BOOLEAN);
        } else if(expr->isAddition()) {
            EVALUATED_TYPE type = ET_INT;
            CompilationTable* string = NULL;
            if(leftExpr->isExprTypeObject()) {
                string = leftExpr->getTableTypeOfExpression();
                if(string->getCanonicalName() == "java.lang.String") {
                    // it's a string
                    type = ET_OBJECT;
                    goto setType;
                }
            }
            
            if(rightExpr->isExprTypeObject()) {
                string = rightExpr->getTableTypeOfExpression();
                if(string->getCanonicalName() == "java.lang.String") {
                    type = ET_OBJECT;
                }
            }
            setType:
            expr->setExprType(type, string);
        } else {
            // everything else is arithmetic, assume the expression type is type int
            expr->setExprType(ET_INT);
        }
    } else if(expr->isAssignName() || expr->isAssignField() || expr->isAssignArray()) {
        traverseAndLink((Assignment*) expr, withinMethod);
    } else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
        traverseAndLink((CastExpression*) expr, withinMethod);
    } else if(expr->isInstanceOf()) {
        traverseAndLink(((InstanceOf*) expr)->getExpressionToCheck(), withinMethod);
        // instance of expressions evaluate to boolean
        // By the design of the language, the type has to be a reference type
        // i.e it is an array of some type whether primitive or referential
        // or it is a type to some class/interface
        expr->setExprType(ET_BOOLEAN);
    } else if(expr->isNameExpression()) {
        Name* name = ((NameExpression*) expr)->getNameExpression();
        traverseAndLink(name, withinMethod);
        // if no error occurred in attempting to link name
        if(!name->postponeLinking()) {
            setExpressionTypeBasedOnName(expr, name);
        }
    } else if(expr->isPrimaryExpression()) {
        Primary* prim = ((PrimaryExpression*) expr)->getPrimaryExpression();
        traverseAndLink(prim, withinMethod);
        // if no error occurred in attempting to link primary
        if(!prim->postponeLinking()) {
            setExpressionTypeBasedOnPrimary(expr, prim);
        }
    } else {
        // all that's left is negation expression, precautionary check
        assert(expr->isNumericNegation() || expr->isBooleanNegation());
        if(expr->isNumericNegation()) { expr->setExprType(ET_INT); }
        else { expr->setExprType(ET_BOOLEAN); }
    }
}

void AmbiguousLinker::traverseAndLink(Assignment* assign, bool withinMethod) {
    if(assign->isAssignName()) {
        traverseAndLink((AssignName*) assign, withinMethod);
    } else if(assign->isAssignField()) {
        traverseAndLink((AssignField*) assign, withinMethod);
    } else {
        // assign array
        traverseAndLink((AssignArray*) assign, withinMethod);
    }
    traverseAndLink(assign->getExpressionToAssign(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(AssignName* assign, bool withinMethod) {
    std::stringstream ss;
    Name* assignName = assign->getNameToAssign();
    traverseAndLink(assignName, withinMethod);
    setExpressionTypeBasedOnName(assign, assignName);
}

void AmbiguousLinker::traverseAndLink(Name* name, bool withinMethod) {
    Name* nextName = NULL;
    if(name == NULL) { return; }
    if(!name->isLastPrefix()) {
        traverseAndLink(name->getNextName(), withinMethod);
    }

    nextName = name->getNextName();
    if(nextName != NULL && !nextName->postponeLinking()) {
        linkQualifiedName(name);
    } else if(nextName == NULL) {
        linkSimpleName(name, withinMethod);
    }
}

void AmbiguousLinker::traverseAndLink(AssignField* assign, bool withinMethod) {
    traverseAndLink(assign->getAssignedField(), withinMethod);
    FieldAccess* accessed = assign->getAssignedField();
    // By the way we do things, acccessed must be pointing to a field
    // OR to array length
    // Following that assumption, precautionary check
    assert(accessed->isReferringToField() || accessed->linkToArrayLength());
    if(accessed->linkToArrayLength()) {
        assign->setExprType(ET_INT);
    } else {
        // referring to a field
        FieldTable* field = accessed->getReferredField();
        setExpressionTypeBasedOnType(assign, field->getField()->getFieldType());
    }
}

void AmbiguousLinker::traverseAndLink(FieldAccess* accessed, bool withinMethod) {
    traverseAndLink(accessed->getAccessedFieldPrimary(), withinMethod);

    Token* tok = accessed->getAccessedFieldId()->getToken();
    std::string currName = tok->getString();
    Primary* prim = accessed->getAccessedFieldPrimary();
    std::stringstream ss;
    
    if(!prim->postponeLinking()) {
        // if primary part was linked
        if(prim->isReferringToField()) {
            FieldDecl* field = prim->getReferredField()->getField();
            if(checkProperMemberAccessingFromVariable(currName, field->getFieldType(), tok)) {
                if(setFieldAccessReferringToArrayLength(accessed, field->getFieldType())) {
                } else {
                    // array length was not set
                }
            }
            // do some more stuff here
        } else if(prim->isReferringToLocalVar()) {
            LocalDecl* localVar = prim->getReferredLocalVar()->getLocalDecl();
            if(checkProperMemberAccessingFromVariable(currName, localVar->getLocalType(), tok)) {
                if(setFieldAccessReferringToArrayLength(accessed, localVar->getLocalType())) {
                } else {
                    // array length was not set
                }
            }
            // do some more stuff here
        } else if(prim->isReferringToParameter()) {
            ParamList* param = prim->getReferredParameter()->getParameter();
            if(checkProperMemberAccessingFromVariable(currName, param->getParameterType(), tok)) {
                if(setFieldAccessReferringToArrayLength(accessed, param->getParameterType())) {
                } else {
                    // array length was not set
                }
            }
            // do some more stuff here
        } else if(prim->isReferringToConstructor()) {
            // a special case, we know then Primary can only be NewClassCreation
            // precautionary check
            assert(prim->isNewClassCreation());
            // safe, we know this is a class since it's checked in A2
            CompilationTable* someClass = ((NewClassCreation*) prim)->getTableOfCreatedClass();
            FieldTable* fieldGotten = getStaticFieldInAClass(someClass, currName, tok);
            if(fieldGotten != NULL) {
                accessed->setReferredField(fieldGotten);
            }
        } else if(prim->linkToPrimitiveLiteral()) {
            LiteralOrThis* lit = (LiteralOrThis*) prim;
            if(lit->isNumber()) {
                ss << "Primitive type integer cannot be dereferenced.";
            } else if(lit->isTrueBoolean() || lit->isFalseBoolean()) {
                ss << "Primitive type boolean cannot be dereferenced.";
            } else if(lit->isCharLiteral()) {
                ss << "Primitive type char cannot be dereferenced.";
            }
            Error(E_DISAMBIGUATION, tok, ss.str());
        } else if(prim->linkToArray()) {
            // then prim must be a PrimaryNewArray
            assert(prim->isNewPrimitiveArray() || prim->isNewReferenceArray());
            Type* type = ((PrimaryNewArray*) prim)->getArrayType();
            if(currName != "length") {
                reportIllegalArrayMemberAccess(currName, type, tok);
            } else {
                // referring to array length
                accessed->setReferringToArrayLength();
            }
        } else if(prim->linkToArrayLength()) {
            // atempting to access array length?
            ss << "Primitive type integer cannot be dereferenced.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        } else if(prim->linkToNull()) {
            ss << "null cannot be dereferenced.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        }
        // defer check for class method to another time
    }
}

void AmbiguousLinker::traverseAndLink(Primary* prim, bool withinMethod) {
    if(prim->isFieldAccess()) {
        traverseAndLink((FieldAccess*) prim, withinMethod);
    } else if(prim->isThis() || prim->isNumber() || prim->isTrueBoolean() || prim->isFalseBoolean()
              || prim->isCharLiteral() || prim->isStringLiteral() || prim->isNull()) {
        traverseAndLink((LiteralOrThis*) prim);
    } else if(prim->isBracketedExpression()) {
        traverseAndLink(((BracketedExpression*) prim)->getExpressionInside(), withinMethod);
        // linking has been resolved, but BracketedExpression doesn't really link to anywhere
        // indicate this
        prim->ResolveLinkButNoEntity();
    } else if(prim->isNewClassCreation()) {
        traverseAndLink((NewClassCreation*) prim, withinMethod);
    } else if(prim->isNewPrimitiveArray() || prim->isNewReferenceArray()) {
        traverseAndLink((PrimaryNewArray*) prim, withinMethod);
    } else if(prim->isNormalMethodCall() || prim->isAccessedMethodCall()) {
        traverseAndLink((MethodInvoke*) prim, withinMethod);
    } else if(prim->isArrayAccessName() || prim->isArrayAccessPrimary()) {
        traverseAndLink((ArrayAccess*) prim, withinMethod);
    } else {
        // the remaining is qualified this, which has already been resolved
        // indicate this
        prim->ResolveLinkButNoEntity();
    }
}

void AmbiguousLinker::traverseAndLink(LiteralOrThis* lit) {
    std::vector<CompilationTable*>::iterator it;
    if(lit->isThis()) {
        lit->setReferredClass(curCompilation);
    } else if(lit->isStringLiteral()) {
        lit->setReferredClass(retrieveCompilation("java.lang", "String"));
    } else if(lit->isNull()) {
        lit->setReferringToNull();
    } else {
        // everything else, i.e a number, true/false, char literal
        lit->setReferringToPrimitiveLiteral();
    }
}

void AmbiguousLinker::traverseAndLink(NewClassCreation* prim, bool withinMethod) {
    traverseAndLink(prim->getArgsToCreateClass(), withinMethod);

    // link constructor later on
}

void AmbiguousLinker::traverseAndLink(ArgumentsStar* args, bool withinMethod) {
    if(!args->isEpsilon()) {
        traverseAndLink(args->getListOfArguments(), withinMethod);
    }
}
void AmbiguousLinker::traverseAndLink(Arguments* args, bool withinMethod) {
    if(!args->lastArgument()) {
        traverseAndLink(args->getNextArgs(), withinMethod);
    }

    traverseAndLink(args->getSelfArgumentExpr(), withinMethod);
    // fill in later, get type of expression
}

void AmbiguousLinker::traverseAndLink(PrimaryNewArray* newArray, bool withinMethod) {
    traverseAndLink(newArray->getTheDimension(), withinMethod);
    newArray->setReferringToArray();
}

void AmbiguousLinker::traverseAndLink(MethodInvoke* invoke, bool withinMethod) {
    if(invoke->isNormalMethodCall()) {
        traverseAndLink((MethodNormalInvoke*) invoke, withinMethod);
    } else {
        traverseAndLink((InvokeAccessedMethod*) invoke, withinMethod);
    }

    traverseAndLink(invoke->getArgsForInvokedMethod(), withinMethod);
    // link method after this
    if(invoke->isNormalMethodCall()) {
    } else {
    }
}

void AmbiguousLinker::traverseAndLink(MethodNormalInvoke* invoke, bool withinMethod) {
    traverseAndLink(invoke->getNameOfInvokedMethod()->getNextName(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(InvokeAccessedMethod* invoke, bool withinMethod) {
    traverseAndLink(invoke->getAccessedMethod()->getAccessedFieldPrimary(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(ArrayAccess* access, bool withinMethod) {
    if(access->isArrayAccessName()) {
        traverseAndLink((ArrayAccessName*) access, withinMethod);
    } else {
        traverseAndLink((ArrayAccessPrimary*) access, withinMethod);
    }

    traverseAndLink(access->getAccessExpression(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(ArrayAccessName* access, bool withinMethod) {
    traverseAndLink(access->getNameOfAccessedArray(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(ArrayAccessPrimary* access, bool withinMethod) {
    traverseAndLink(access->getAccessedPrimaryArray(), withinMethod);
}

void AmbiguousLinker::traverseAndLink(AssignArray* assign, bool withinMethod) {
    std::stringstream ss;
    ArrayAccess* assignArray = assign->getAssignedArray();
    traverseAndLink(assignArray, withinMethod);
    if(assignArray->isArrayAccessName()) {
        Name* nameArray = ((ArrayAccessName*) assignArray)->getNameOfAccessedArray();
        Token* tok = nameArray->getNameId()->getToken();
        setExpressionTypeBasedOnName(assign, nameArray);
        // check that nameArray type was actually an array, by
        // seeing what assign's type was configured to
        checkProperArrayAccessInExpression(assign, tok);
    } else if(assignArray->isArrayAccessPrimary()) {
        Primary* prim = ((ArrayAccessPrimary*) assignArray)->getAccessedPrimaryArray();
        Token* tok = setExpressionTypeBasedOnPrimary(assign, prim);
        checkProperArrayAccessInExpression(assign, tok);
    }
}

void AmbiguousLinker::traverseAndLink(CastExpression* cast, bool withinMethod) {
    if(cast->isCastToArrayName()) {
        traverseAndLink((CastName*) cast, withinMethod);
    } else if(cast->isCastToReferenceType()) {
        traverseAndLink((CastPrimitive*) cast);
    }
}

void AmbiguousLinker::traverseAndLink(CastName* cast, bool withinMethod) {
    Name* nameToCastTo = cast->getNameToCastTo();
    traverseAndLink(nameToCastTo, withinMethod);
    // an error in linking name, quietly return
    if(nameToCastTo->postponeLinking()) { return; }

    if(!nameToCastTo->isReferringToType()) {
        // name links to some class/interface
        setExpressionTypeBasedOnName(cast, nameToCastTo);
        if(!cast->isExprTypeNotEvaluated()) {
            // if the type can be evaluated, do the following
            // precuationary check, the type of cast should now be 
            // some class/interface
            assert(cast->isExprTypeObject());
            if(cast->isCastToArrayName()) {
                // reconfigure if this is a cast to an array of class
                // or interface
                cast->reconfigureExprType(ET_OBJECTARRAY);
            }
        }
    } else {
        // attempting to cast to something which is not
        // a class/object
        std::stringstream ss;
        ss << "Attempting to cast to a class or interface, but instead got '"
           << nameToCastTo->getFullName() << "'.";
    }
}

void AmbiguousLinker::traverseAndLink(CastPrimitive* cast) {
    PrimitiveType* type = cast->getPrimitiveTypeToCastTo();
    setExpressionTypeBasedOnType(cast, type);
}

// ----------------------------------------------------------------------------
// Interface Part

void AmbiguousLinker::traverseAndLink(InterfaceDecl* anInterface) {}

void AmbiguousLinker::traverseAndLink(CompilationTable* compilation) {
    curCompilation = compilation;
    curSymTable = compilation->getSymbolTable();
    if(curSymTable == NULL) {
        // just return since there is nothing to do
        return;
    }

    if(curSymTable->isClassTable()) {
        traverseAndLink(((ClassTable*) curSymTable)->getClass());
    } else {
        // must be an interface table
        traverseAndLink(((InterfaceTable*) curSymTable)->getInterface());
    }
}

// ----------------------------------------------------------------------------
// Error reporting
void AmbiguousLinker::reportIllegalPrimitiveMemberAccess(const std::string& memberName, PrimitiveType* type, Token* tok) {
    if(type->isPrimitiveArray()) {
        reportIllegalArrayMemberAccess(memberName, type, tok);
    } else {
        std::stringstream ss;
        ss << "Primitive type '" << type->getTypeAsString() << "' cannot be dereferenced.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    }
}

void AmbiguousLinker::reportIllegalArrayMemberAccess(const std::string& memberName, Type* type, Token* tok) {
    std::stringstream ss;
    ss << "Attempting to access member '" << memberName << "' from array '"
       << type->getTypeAsString() << "' which is not length.";
    Error(E_DISAMBIGUATION, tok, ss.str());
}

// ----------------------------------------------------------------------------
// Various helpers
bool AmbiguousLinker::checkProperMemberAccessingFromVariable(const std::string& currName, Type* type, Token* tok) {
    bool illegal = false;
    if(type->isPrimitiveType()) {
        if(((PrimitiveType*) type)->isPrimitiveArray()) { 
            if(currName != "length") {
                illegal = true;
            }
        } else {
            illegal = true;
            // a primitive type that is not an array
        }

        if(illegal) {
            reportIllegalPrimitiveMemberAccess(currName, (PrimitiveType*) type, tok);
        }
    } else {
        // then it must be a reference type
        ReferenceType* obj = (ReferenceType*) type;
        std::stringstream ss;
        if(obj->isReferenceArrayType()) {
            // it's an array of some objects
            if(currName != "length") {
                // accessing a member that is not length from an array
                ss << "Attempting to access member '" << currName << "' from array '"
                   << obj->getTypeAsString() << "' which is not length.";
                Error(E_DISAMBIGUATION, tok, ss.str());
                illegal = true;
            }
        } else {
            // it's some type, check that it is not an interface
            // because interfaces don't have fields
            if(!obj->getReferenceTypeTable()->isClassSymbolTable()) {
                // it's an interface
                ss << "Attempting to access a non-static field '" << currName << "' from interface '"
                   << obj->getReferenceTypeTable()->getCanonicalName() << "', but interfaces have no non-static field.";
                Error(E_DISAMBIGUATION, tok, ss.str());
                illegal = true;
            }
        }
    }
    return !illegal;
}

bool AmbiguousLinker::checkTypeIsClassDuringStaticAccess(CompilationTable* typeTable, const std::string& fullName, Token* tok) {
    if(!typeTable->isClassSymbolTable()) {
        std::stringstream ss;
        ss << "Attempting to access static member of interface '" << fullName
           << "'. Joos 1W has no interface constants.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        return false;
    }
    return true;
}

FieldTable* AmbiguousLinker::findFieldPreviouslyDeclared(const std::string& fieldName) {
    SymbolTable* symTable = curSymTable;
    while(symTable != NULL) {
        if(symTable->isFieldTable()) {
            if(((FieldTable*) symTable)->getField()->getFieldDeclared()->getIdAsString() == fieldName) {
                return (FieldTable*) symTable;
            }
        }
        symTable = symTable->getPrevTable();
    }
    // if we ever reach here, then symTable must be NULL
    return NULL;
}

CompilationTable* AmbiguousLinker::findTypeFromSingleImportsAndPackage(const std::string& typeName, Token* tok) {
    CompilationTable* retCompilation = NULL;
    // from single import
    retCompilation = curCompilation->checkTypePresenceFromSingleImport(typeName);
    if(retCompilation != NULL) { return retCompilation; }
    
    // from package in other compilation units
    Package* pkg = manager.getPackageViaName(curCompilation->getPackageRawForm()->getPackageName());
    retCompilation = pkg->getACompilationWithName(typeName);
    if(retCompilation != NULL) { return retCompilation; }

    // from type import on demand
    retCompilation = curCompilation->checkTypePresenceFromImportOnDemand(typeName, tok);
    
    // return result, may be NULL
    return retCompilation;
}

void AmbiguousLinker::linkQualifiedName(Name* name) {
    // if this is a qualifying name and the qualifier has been linked
    // to some entity (aside from method and non-static members) then do these checks
    // 
    // Order is based from JLS 6.5.2
    Token* tok = name->getNameId()->getToken();
    std::string currName = tok->getString();
    Name* nextName = name->getNextName();
    std::stringstream ss;
    if(nextName->isReferringToPackage()) {
        // if nothing else the name is referring to a package
        Package* pkg = nextName->getReferredPackage();
        // assumption is that no package names resolve to types
        Package* subPkg = pkg->getSubpackageWithName(currName);
        if(subPkg != NULL) {
            // then this refers to a subpackage
            name->setReferredPackage(subPkg);
        } else {
            // then this should be a type
            CompilationTable* typeTable = pkg->getACompilationWithName(currName);
            if(typeTable == NULL) {
                // if it's still NULL, then error
                std::stringstream ss;
                ss << "No subpackage or type '" << currName << "' can be identified via package '"
                   << nextName->getFullName() << "'.";
                Error(E_DISAMBIGUATION, tok, ss.str());
            }
        }
    } else if(nextName->isReferringToType()) {
        CompilationTable* someType = nextName->getReferredType();
        // check that typeTable refers to a class
        if(checkTypeIsClassDuringStaticAccess(someType, currName, tok)) {
            // it passed therefore it is a class, can safely do this
            FieldTable* fieldFound = someType->getAField(currName);
            if(fieldFound != NULL) {
                // field was found
                name->setReferredField(fieldFound);
            }
        }
        // the rest are what is categorized in the JLS as ExpressionName
    } else if(nextName->isReferringToField()) {
        FieldDecl* field = nextName->getReferredField()->getField();
        if(checkProperMemberAccessingFromVariable(currName, field->getFieldType(), tok)) {
            if(setNameReferringToArrayLength(name, field->getFieldType())) {
            } else {
            }
        }
        // do some more stuff here
    } else if(nextName->isReferringToLocalVar()) {
        LocalDecl* localVar = nextName->getReferredLocalVar()->getLocalDecl();
        if(checkProperMemberAccessingFromVariable(currName, localVar->getLocalType(), tok)) {
            if(setNameReferringToArrayLength(name, localVar->getLocalType())) {
            } else {
            }
        }
        // do some more stuff here
    } else if(nextName->isReferringToParameter()) {
        ParamList* param = nextName->getReferredParameter()->getParameter();
        if(checkProperMemberAccessingFromVariable(currName, param->getParameterType(), tok)) {
            if(setNameReferringToArrayLength(name, param->getParameterType())) {
            } else {
            }
        }
        // do some more stuff here
    } else if(nextName->linkToArrayLength()) {
        // atempting to access array length?
        ss << "Primitive type int cannot be dereferenced.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    }
    // defer check for method at a a different stage
}

void AmbiguousLinker::linkSimpleName(Name* name, bool withinMethod) {
    // pretty much a simple name, then do these checks
    // Order based on JLS 6.5.2
    
    Token* tok = name->getNameId()->getToken();
    std::string currName = tok->getString();
    if(withinMethod) {
        // the check is occurring inside a method
    } else {
        FieldTable* field = findFieldPreviouslyDeclared(currName);
        if(field != NULL) {
            // the field was found
            name->setReferredField(field);
            return;
        }
    }

    if(currName == curCompilation->getClassOrInterfaceName()) {
        // it is the name of the current type
        name->setReferredType(curCompilation);
        return;
    }

    CompilationTable* typeTable = findTypeFromSingleImportsAndPackage(currName, tok);
    if(typeTable != NULL) {
        // type was found from single type imports or package
        name->setReferredType(typeTable);
        return;
    }

    Package* pkg = manager.getPackageViaName(name);
    if(pkg != NULL) {
        name->setReferredPackage(pkg);
        return;
    }
    
    // if it's none of the above, then error
    std::stringstream ss;
    ss << "Simple name '" << currName << "' does not refer to either a local variable, parameter, field, type nor package.";
    Error(E_DISAMBIGUATION, tok, ss.str());
}

FieldTable* AmbiguousLinker::getStaticFieldInAClass(CompilationTable* someClass, const std::string& findField, Token* tok) {
    FieldTable* field = someClass->getAField(findField);
    if(field == NULL) {
        // if the field cannot be found
        std::stringstream ss;
        ss << "Cannot locate static field '" << findField << "' in class '" << someClass->getCanonicalName() << "'.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    }
    return field;
}

CompilationTable* AmbiguousLinker::retrieveCompilation(const std::string& package, const std::string& typeName) {
    CompilationTable* table = NULL;
    std::vector<CompilationTable*>::iterator it;
    for(it = compilations[package].begin(); it != compilations[package].end(); it++) {
        if((*it)->getClassOrInterfaceName() == typeName) {
            table = *it;
            break;
        }
    }
    return table;
}

bool AmbiguousLinker::setNameReferringToArrayLength(Name* name, Type* type) {
    if((type->isPrimitiveType() && ((PrimitiveType*) type)->isPrimitiveArray())
       || type->isReferenceArrayType()) {
        // either a primitive array or an array of some class/interface
        name->setReferringToArrayLength();
        return true;
    }
    return false;
}

bool AmbiguousLinker::setFieldAccessReferringToArrayLength(FieldAccess* access, Type* type) {
    if((type->isPrimitiveType() && ((PrimitiveType*) type)->isPrimitiveArray())
       || type->isReferenceArrayType()) {
        // either a primitive array or an array of some class/interface
        access->setReferringToArrayLength();
        return true;
    }
    return false;
}

int AmbiguousLinker::returnEvalTypeFromTypeNode(Type* type) {
    EVALUATED_TYPE exprType = ET_NOTEVALUATED;
    if(type->isPrimitiveType()) {
        PrimitiveType* primType = (PrimitiveType*) type;
        if(primType->isTypeInt()) {
            exprType = ET_INT;
        } else if(primType->isTypeShort()) {
            exprType = ET_SHORT;
        } else if(primType->isTypeByte()) {
            exprType = ET_BYTE;
        } else if(primType->isTypeChar()) {
            exprType = ET_CHAR;
        } else {
            exprType = ET_BOOLEAN;
        }

        if(primType->isPrimitiveArray()) {
            // make it into an array type
            return exprType+5;
        } else { return exprType; }
    } else {
        if(type->isReferenceArrayType()) {
            return ET_OBJECTARRAY;
        } else {
            return ET_OBJECT;
        }
    }
}

void AmbiguousLinker::setExpressionTypeBasedOnType(Expression* expr, Type* type) {
    EVALUATED_TYPE exprType = ET_NOTEVALUATED;
    CompilationTable* table = NULL;
    exprType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
    if(exprType == ET_OBJECT || exprType == ET_OBJECTARRAY) {
        table = ((ReferenceType*) type)->getReferenceTypeTable();
    }
    expr->setExprType(exprType, table);
}

void AmbiguousLinker::setExpressionTypeBasedOnName(Expression* expr, Name* name) {
    // postponed? -> then there was a problem, if so return
    if(name->postponeLinking()) { return; }
    
    std::stringstream ss;
    Token* tok = name->getNameId()->getToken();
    Type* type = NULL;
    if(name->isReferringToType()) {
        ss << "Incomplete use of type name '" << name->getFullName()
           << "'. Were you trying to access a static member?";
        Error(E_DISAMBIGUATION, tok, ss.str());
        return;
    } else if(name->isReferringToField()) {
        FieldTable* field = name->getReferredField();
        type = field->getField()->getFieldType();
    } else if(name->isReferringToParameter()) {
        ParamTable* param = name->getReferredParameter();
        type = param->getParameter()->getParameterType();
    } else if(name->isReferringToLocalVar()) {
        LocalTable* local = name->getReferredLocalVar();
        type = local->getLocalDecl()->getLocalType();
    } else if(name->linkToArrayLength()) {
        expr->setExprType(ET_INT);
        return;
    } else {
        // referring to a package
        ss << "Incomplete use of package name '" << name->getFullName()
           << "'. Were you trying to access a type from that package?";
        Error(E_DISAMBIGUATION, tok, ss.str());
        return;
    }

    setExpressionTypeBasedOnType(expr, type);
}

void AmbiguousLinker::checkProperArrayAccessInExpression(Expression* expr, Token* tok) {
    // check if the expression type is set, if not
    // then there must have been an error before
    if(expr->isExprTypeNotEvaluated()) { return; }
    std::stringstream ss;
    ss << "Expected an array but instead got ";
    if(expr->isExprTypeInt()) {
        ss << "int.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeChar()) {
        ss << "char.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeShort()) {
        ss << "short.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeByte()) {
        ss << "byte.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeBoolean()) {
        ss << "boolean.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeObject()) {
        ss << "'" << expr->getTableTypeOfExpression()->getCanonicalName() << "'.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        // the above was error part
        // now everything is an array, thus need to
        // assign the correct type to assignArray since this is
        // an array access
    } else if(expr->isExprTypeIntArray()) {
        expr->reconfigureExprType(ET_INT);
    } else if(expr->isExprTypeShortArray()) {
        expr->reconfigureExprType(ET_SHORT);
    } else if(expr->isExprTypeByteArray()) {
        expr->reconfigureExprType(ET_BYTE);
    } else if(expr->isExprTypeCharArray()) {
        expr->reconfigureExprType(ET_CHAR);
    } else {
        // an array of some type
        expr->reconfigureExprType(ET_OBJECT);
    }
}

Token* AmbiguousLinker::setExpressionTypeBasedOnPrimary(Expression* expr, Primary* prim) {
    // linking was not done, indicates a previous error, immediately return
    Token* tok = NULL;
    Type* type = NULL;
    if(prim->postponeLinking()) { return tok; }

    if(prim->isReferringToField()) {
        // then we know this can only be field access
        type = prim->getReferredField()->getField()->getFieldType();
        tok = ((FieldAccess*) prim)->getAccessedFieldId()->getToken();
        setExpressionTypeBasedOnType(expr, type);
    } else if(prim->isReferringToClassMethod() || prim->isReferringToInterfaceMethod()) {
        // either MethodNormalInvoke or InvokeAccessedMethod
        if(prim->isReferringToClassMethod()) {
            type = prim->getReferredClassMethod()->getClassMethod()->getMethodHeader()->getReturnType();
        } else { type = prim->getReferredInterfaceMethod()->getInterfaceMethod()->getReturnType(); }

        if(prim->isNormalMethodCall()) {
            tok = ((MethodNormalInvoke*) prim)->getNameOfInvokedMethod()->getNameId()->getToken();
        } else {
            // InvokeAccessedMethod
            tok = ((InvokeAccessedMethod*) prim)->getAccessedMethod()->getAccessedFieldId()->getToken();
        }
        setExpressionTypeBasedOnType(expr, type);
    } else if(prim->isReferringToParameter() || prim->isReferringToLocalVar()) {
        // must be an array access name
        if(prim->isReferringToParameter()) {
            type = prim->getReferredParameter()->getParameter()->getParameterType();
        } else { type = prim->getReferredLocalVar()->getLocalDecl()->getLocalType(); }
        
        tok = ((ArrayAccessName*) prim)->getNameOfAccessedArray()->getNameId()->getToken();
        setExpressionTypeBasedOnType(expr, type);
    } else if(prim->isReferringToConstructor()) {
        // then we know this can only be NewClassCreation
        // precuationary check
        assert(prim->isNewClassCreation());
        expr->setExprType(ET_OBJECT, ((NewClassCreation*) prim)->getTableOfCreatedClass());
        tok = ((NewClassCreation*) prim)->getClassName()->getNameId()->getToken();
    } else if(prim->linkToPrimitiveLiteral() || prim->linkToNull()) {
        // must be LiteralOrThis BUT not this
        assert(prim->isNumber() || prim->isTrueBoolean() || prim->isFalseBoolean() || prim->isCharLiteral() || prim->isNull());
        if(prim->isNumber()) { expr->setExprType(ET_INT); }
        else if(prim->isTrueBoolean() || prim->isFalseBoolean()) { expr->setExprType(ET_BOOLEAN); }
        else if(prim->isCharLiteral()) { expr->setExprType(ET_CHAR); }
        else { expr->setExprType(ET_NULL); }

        tok = ((LiteralOrThis*) prim)->getLiteralToken();
    } else if(prim->linkToArray()) {
        // then we know this can only be PrimaryNewArray
        // precuationary check
        assert(prim->isNewPrimitiveArray() || prim->isNewReferenceArray());
        type = ((PrimaryNewArray*) prim)->getArrayType();
        setExpressionTypeBasedOnType(expr, type);
        if(expr->isExprTypeInt() || expr->isExprTypeShort() || expr->isExprTypeByte() ||
           expr->isExprTypeChar() || expr->isExprTypeBoolean()) {
            // turn to an array
            expr->reconfigureExprType((EVALUATED_TYPE) (expr->getExprType()+5));
        } else {
            // if none of the above, should be an object, modifify to an array of objects
            assert(expr->isExprTypeObject());
            expr->reconfigureExprType(ET_OBJECTARRAY);
        }
        if(type->isPrimitiveType()) {
            tok = ((PrimitiveType*) type)->getPrimTypeToken();
        } else {
            tok = ((ReferenceType*) type)->getReferenceName()->getNameId()->getToken();
        }
    } else if(prim->resolvedLinkButNoEntity()) {
        // either QualifiedThis or BracketedExpression
        if(prim->isBracketedExpression()) {
            Expression* bracketed = ((BracketedExpression*) prim)->getExpressionInside();
            // just copy the type
            expr->setExprType(bracketed->getExprType(), bracketed->getTableTypeOfExpression());
            // getting a Token from an expression is too bothersome lol, maybe some other time
        } else {
            // QualifiedThis
            expr->setExprType(ET_OBJECT, ((QualifiedThis*) prim)->getQualifyingClassTable());
            tok = ((QualifiedThis*) prim)->getQualifyingClassName()->getNameId()->getToken();
        }
    } else {
        // the only one remaining, precautionary check
        assert(prim->linkToArrayLength() && prim->isFieldAccess());
        expr->setExprType(ET_INT);
        tok = ((FieldAccess*) prim)->getAccessedFieldId()->getToken();
    }

    return tok;
}

// --------------------------------------------------------------------------------------------

void AmbiguousLinker::performLinking() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            traverseAndLink(*it2);
        }
    }
}
