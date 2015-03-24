#include <sstream>
#include <cassert>
#include <iostream>

#include "error.h"
#include "package.h"
#include "typeString.h"

#include "ambiguousLinkType.h"
#include "ambiguousLinker.h"
#include "paramTable.h"
#include "classMethodTable.h"
#include "interfaceMethodTable.h"
#include "forTable.h"
#include "constructorTable.h"

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
#include "blockStmtsStar.h"
#include "blockStmts.h"
#include "localDecl.h"
#include "formalParamStar.h"
#include "paramList.h"
#include "returnStmt.h"
#include "expressionStar.h"
#include "stmtExprAssign.h"
#include "stmtExprCreation.h"
#include "stmtExprInvoke.h"
#include "ifStmt.h"
#include "whileStmt.h"
#include "forStmt.h"
#include "nestedBlock.h"
#include "constructor.h"
#include "negationExpression.h"

#include "interfaceDecl.h"

AmbiguousLinker::AmbiguousLinker(PackagesManager& manager, std::map<std::string, std::vector<CompilationTable*> >& compilations) :
   manager(manager), compilations(compilations), curSymTable(NULL), curCompilation(NULL), 
   asgmtFieldContext(false), asgmtLocalContext(false), withinMethod(false) {}

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
        withinMethod = true;
        traverseAndLink((ClassMethod*) body);
        // reset
        withinMethod = false;
    } else {
        // constructor
        withinMethod = true;
        traverseAndLink((Constructor*) body);
        // reset
        withinMethod = false;
    }
}

void AmbiguousLinker::traverseAndLink(FieldDecl* field) {
    if(field->isInitialized()) {
        traverseAndLink(field->getInitializingExpression());
    }
    // update symbol table
    curSymTable = field->getFieldTable();
}

void AmbiguousLinker::traverseAndLink(Expression* expr) {
    if(expr->isRegularBinaryExpression()) {
        Expression* leftExpr = ((BinaryExpression*) expr)->getLeftExpression();
        Expression* rightExpr = ((BinaryExpression*) expr)->getRightExpression();
        traverseAndLink(leftExpr);
        traverseAndLink(rightExpr);
        if(expr->isLazyOr() || expr->isLazyAnd() || expr->isEagerOr() || expr->isEagerAnd() ||
           expr->isEqual() || expr->isNotEqual() || expr->isLT() || expr->isGT() || expr->isLTE() || expr->isGTE() ||
           expr->isInstanceOf()) {
            // specualate for now that expression type is boolean
            expr->setExprType(ET_BOOLEAN);
        } else if(expr->isAddition()) {
            // by default the type of addition is int
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
        traverseAndLink((Assignment*) expr);
    } else if(expr->isCastToArrayName() || expr->isCastToReferenceType() || expr->isCastToPrimitiveType()) {
        traverseAndLink((CastExpression*) expr);
    } else if(expr->isInstanceOf()) {
        traverseAndLink(((InstanceOf*) expr)->getExpressionToCheck());
        // instance of expressions evaluate to boolean
        // By the design of the language, the type has to be a reference type
        // i.e it is an array of some type whether primitive or referential
        // or it is a type to some class/interface
        expr->setExprType(ET_BOOLEAN);
    } else if(expr->isNameExpression()) {
        Name* name = ((NameExpression*) expr)->getNameExpression();
        traverseAndLink(name);
        setExpressionTypeBasedOnName(expr, name);
    } else if(expr->isPrimaryExpression()) {
        Primary* prim = ((PrimaryExpression*) expr)->getPrimaryExpression();
        traverseAndLink(prim);
        setExpressionTypeBasedOnPrimary(expr, prim);
    } else {
        // all that's left is negation expression, precautionary check
        assert(expr->isNumericNegation() || expr->isBooleanNegation());
        traverseAndLink(((NegationExpression*) expr)->getNegatedExpression());
        if(expr->isNumericNegation()) { expr->setExprType(ET_INT); }
        else { expr->setExprType(ET_BOOLEAN); }
    }
}

void AmbiguousLinker::traverseAndLink(Assignment* assign) {
    if(assign->isAssignName()) {
        // indicate that this assignment may occur in
        // a FieldDecl initializer expression
        // True if we are not in a method, False otherwise
        
        // store the current value of asgmtFieldContext
        bool tmpAsgmtContext = asgmtFieldContext;
        asgmtFieldContext = !withinMethod;
        traverseAndLink((AssignName*) assign);
        // reset
        asgmtFieldContext = tmpAsgmtContext;
    } else if(assign->isAssignField()) {
        traverseAndLink((AssignField*) assign);
    } else {
        // assign array
        traverseAndLink((AssignArray*) assign);
    }
    traverseAndLink(assign->getExpressionToAssign());
}

void AmbiguousLinker::traverseAndLink(AssignName* assign) {
    Name* assignName = assign->getNameToAssign();
    traverseAndLink(assignName);
    setExpressionTypeBasedOnName(assign, assignName);
}

void AmbiguousLinker::traverseAndLink(Name* name) {
    Name* nextName = NULL;
    if(name == NULL) { return; }
    if(!name->isLastPrefix()) {
        traverseAndLink(name->getNextName());
    }

    nextName = name->getNextName();
    if(nextName != NULL && !nextName->postponeLinking()) {
        linkQualifiedName(name);
    } else if(nextName == NULL) {
        // if name is simple name
        linkSimpleName(name);
    }
}

void AmbiguousLinker::traverseAndLink(AssignField* assign) {
    traverseAndLink(assign->getAssignedField());
    FieldAccess* accessed = assign->getAssignedField();
    // checked if FieldAccess can be linked
    if(!accessed->postponeLinking()) {
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
}

void AmbiguousLinker::traverseAndLink(FieldAccess* accessed) {
    traverseAndLink(accessed->getAccessedFieldPrimary());

    Token* tok = accessed->getAccessedFieldId()->getToken();
    std::string currName = tok->getString();
    Primary* prim = accessed->getAccessedFieldPrimary();
    std::stringstream ss;
    
    if(!prim->postponeLinking()) {
        // if primary part was linked
        CompilationTable* someClass = NULL;
        Type* type = NULL;
        if(prim->isReferringToClass()) {
            someClass = prim->getReferredClass();
            linkFieldAccessFromCompilation(accessed, someClass, currName, tok);
        } else if(prim->isReferringToField()) {
            FieldDecl* field = prim->getReferredField()->getField();
            if(checkProperMemberAccessingFromVariable(currName, field->getFieldType(), tok)) {
                if(!setFieldAccessReferringToArrayLength(accessed, field->getFieldType())) {
                    // did not refer to array length
                    linkFieldAccessFromType(accessed, (ReferenceType*) field->getFieldType(), currName);
                }
            }
        } else if(prim->isReferringToConstructor()) {
            // a special case, we know then Primary can only be NewClassCreation
            // precautionary check
            assert(prim->isNewClassCreation());
            // safe, we know this is a class since it's checked in A2
            someClass = ((NewClassCreation*) prim)->getTableOfCreatedClass();
            linkFieldAccessFromCompilation(accessed, someClass, currName, tok);
        } else if(prim->linkToPrimitiveLiteral()) {
            // then must be LiteralOrThis
            reportLiteralDereferencing((LiteralOrThis*) prim, tok);
        } else if(prim->linkToArray()) {
            // then prim must be a PrimaryNewArray
            assert(prim->isNewPrimitiveArray() || prim->isNewReferenceArray());
            type = ((PrimaryNewArray*) prim)->getArrayType();
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
            reportLiteralDereferencing((LiteralOrThis*) prim, tok);
        } else if(prim->isReferringToClassMethod()) {
            type = prim->getReferredClassMethod()->getClassMethod()->getMethodHeader()->getReturnType();
            if(checkProperMemberAccessingFromVariable(currName, type, tok)) {
                if(!setFieldAccessReferringToArrayLength(accessed, type)) {
                    // did not refer to array length
                    linkFieldAccessFromType(accessed, (ReferenceType*) type, currName);
                }
            }
        } else if(prim->isReferringToInterfaceMethod()) {
            type = prim->getReferredInterfaceMethod()->getInterfaceMethod()->getReturnType();
            if(checkProperMemberAccessingFromVariable(currName, type, tok)) {
                if(!setFieldAccessReferringToArrayLength(accessed, type)) {
                    // did not refer to array length
                    linkFieldAccessFromType(accessed, (ReferenceType*) type, currName);
                }
            }
        } else {
            assert(prim->resolvedLinkButNoEntity());
            // three checks
            std::stringstream ss;
            if(prim->isBracketedExpression()) {
                Expression* bracketed = ((BracketedExpression*) prim)->getExpressionInside();
                if(!bracketed->isExprTypeNotEvaluated()) {
                    if(!bracketed->isExprTypeObject()) {
                        if(bracketed->isExprTypeIntArray() || bracketed->isExprTypeShortArray() 
                           || bracketed->isExprTypeByteArray() || bracketed->isExprTypeCharArray()
                           || bracketed->isExprTypeBooleanArray() || bracketed->isExprTypeObjectArray()) {
                            if(currName != "length") {
                                ss << "Attempting to access a member of array '"
                                   << bracketed->getExpressionTypeString() << "' that is not 'length'.";
                                Error(E_DISAMBIGUATION, tok, ss.str());
                            } else {
                                // it is an array and the current member accessed is
                                // length
                                prim->linkToArrayLength();
                            }
                        } else {
                            ss << "Expecting a class but instead got '"
                               << bracketed->getExpressionTypeString() << "'.";
                            Error(E_DISAMBIGUATION, tok, ss.str());
                        }
                    } else {
                        // well it's an object, now let's see if we can access it's field and set it
                        CompilationTable* someClass = bracketed->getTableTypeOfExpression();
                        linkFieldAccessFromCompilation(accessed, someClass, currName, tok);
                    }
                }
            } else if(prim->isQualifiedThis()) {
                // QualifiedThis
                linkFieldAccessFromCompilation(accessed, curCompilation, currName, tok);
            } else {
                // Array Access
                assert(prim->isArrayAccessName() || prim->isArrayAccessPrimary());
                ArrayAccess* accessPrim = (ArrayAccess*) prim;
                someClass = accessPrim->getTableOfArrayObjects();
                if(someClass != NULL) {
                    if(!someClass->isClassSymbolTable()) {
                        // it's an interface
                        ss << "Attempting to access field member '" << currName
                           << "' of interface.";
                        Error(E_DISAMBIGUATION, tok, ss.str());
                    } else {
                        linkFieldAccessFromCompilation(accessed, someClass, currName, tok);
                    }
                } else {
                    // since someType == NULL, it must mean that the accessed array element
                    // is not a class or interface type
                    ss << "Expecting a class but instead got '"
                       << getCorrespondingTypeString(accessPrim->getTypeOfArrayElements(), accessPrim->getTableOfArrayObjects())
                       << "'.";
                    Error(E_DISAMBIGUATION, tok, ss.str());
                }
            }
        }
    }
}

void AmbiguousLinker::traverseAndLink(Primary* prim) {
    if(prim->isFieldAccess()) {
        traverseAndLink((FieldAccess*) prim);
    } else if(prim->isThis() || prim->isNumber() || prim->isTrueBoolean() || prim->isFalseBoolean()
              || prim->isCharLiteral() || prim->isStringLiteral() || prim->isNull()) {
        traverseAndLink((LiteralOrThis*) prim);
    } else if(prim->isBracketedExpression()) {
        traverseAndLink((BracketedExpression*) prim);
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
    }
}

void AmbiguousLinker::traverseAndLink(LiteralOrThis* lit) {
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

void AmbiguousLinker::traverseAndLink(BracketedExpression* brkExpr) {
    traverseAndLink(brkExpr->getExpressionInside());
    if(!brkExpr->getExpressionInside()->isExprTypeNotEvaluated()) {
        // expression type can be evaluated
        // indicate that this bracketed expression has been
        // successfully resolved, even though it links to no entity
        brkExpr->ResolveLinkButNoEntity();
    }
}

void AmbiguousLinker::traverseAndLink(NewClassCreation* prim) {
    // no need to link the name, because we know the type already,
    // and that it links to a freaking class too
    traverseAndLink(prim->getArgsToCreateClass());
    
    // link constructor later on
    Token* tok = prim->getClassName()->getNameId()->getToken();
    std::string ctorSignature = prim->constructorInvocationMatchToSignature();
    if(checkProperMethodOrConstructorSignature(ctorSignature, tok)) {
        CompilationTable* someClass = prim->getTableOfCreatedClass();
        ConstructorTable* ctorTable = someClass->getAConstructor(ctorSignature);
        if(ctorTable != NULL) {
            prim->setReferredConstructor(ctorTable);
        } else {
            std::stringstream ss;
            ss << "Constructor of class '" << someClass->getCanonicalName()
               << "' with signature '" << prim->getClassName()->getNameId()->getIdAsString()
               << ctorSignature << "' could not be found.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        }
    }
}

void AmbiguousLinker::traverseAndLink(ArgumentsStar* args) {
    if(!args->isEpsilon()) {
        traverseAndLink(args->getListOfArguments());
    }
}
void AmbiguousLinker::traverseAndLink(Arguments* args) {
    if(!args->lastArgument()) {
        traverseAndLink(args->getNextArgs());
    }

    traverseAndLink(args->getSelfArgumentExpr());
    // fill in later, get type of expression
}

void AmbiguousLinker::traverseAndLink(PrimaryNewArray* newArray) {
    traverseAndLink(newArray->getTheDimension());
    newArray->setReferringToArray();
}

void AmbiguousLinker::traverseAndLink(MethodInvoke* invoke) {
    // traverse the arguments first because we need it later on
    traverseAndLink(invoke->getArgsForInvokedMethod());

    if(invoke->isNormalMethodCall()) {
        traverseAndLink((MethodNormalInvoke*) invoke);
    } else {
        traverseAndLink((InvokeAccessedMethod*) invoke);
    }
}

void AmbiguousLinker::traverseAndLink(MethodNormalInvoke* invoke) {
    Name* invokedName = invoke->getNameOfInvokedMethod();
    Token* tok = invokedName->getNameId()->getToken();
    traverseAndLink(invokedName->getNextName());
    
    std::string methodSignature = invoke->methodInvocationMatchToSignature();
    CompilationTable* someType = NULL;
    if(!checkProperMethodOrConstructorSignature(methodSignature, tok)) {
        // improper method signature, either:
        // - it contained void/null
        // - or one of the arguments' type cannot be retrieved
        return;
    }
    
    if(invokedName->isSimpleName()) {
        // then the method call should be of this class's
        setMethodForMethodInvokeFromCompilation(invoke, curCompilation, methodSignature, tok); 
    } else {
        // qualified name
        Type* type = NULL;
        Name* nextName = invokedName->getNextName();
        if(!nextName->postponeLinking()) {
            // nextName was successfully linked to some entity, check for this entity
            if(nextName->isReferringToType()) {
                someType = nextName->getReferredType();
                if(checkTypeIsClassDuringStaticAccess(someType, nextName->getFullName(), tok)) {
                    // it is a class, then it's find to do the following
                    // whether the method is static or not is not checked here
                    // note that though Interfaces have java.lang.Object's methods,
                    // java.lang.Object has no static methods
                    setMethodForMethodInvokeFromCompilation(invoke, someType, methodSignature, tok);
                }
            } else if(nextName->isReferringToField()) {
                type = nextName->getReferredField()->getField()->getFieldType();
                setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
            } else if(nextName->isReferringToLocalVar()) {
                type = nextName->getReferredLocalVar()->getLocalDecl()->getLocalType();
                setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
            } else if(nextName->isReferringToParameter()) {
                type = nextName->getReferredParameter()->getParameter()->getParameterType();
                setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
            } else if(nextName->isReferringToPackage()) {
                reportIncompletePackageNameUsage(nextName->getFullName(), tok);
            } else {
                // nextName must have been linked to array length
                // precautionary check
                assert(nextName->linkToArrayLength());
                std::stringstream ss;
                ss << "Attempting to access method member of type 'int'.";
                Error(E_DISAMBIGUATION, tok, ss.str());
            } 
        }
    }
}

void AmbiguousLinker::traverseAndLink(InvokeAccessedMethod* invoke) {
    Primary* prim = invoke->getAccessedMethod()->getAccessedFieldPrimary();
    Token* tok = invoke->getAccessedMethod()->getAccessedFieldId()->getToken();
    traverseAndLink(prim);

    std::string methodSignature = invoke->methodInvocationMatchToSignature();
    CompilationTable* someType = NULL;
    Type* type = NULL;

    if(!checkProperMethodOrConstructorSignature(methodSignature, tok)) { return; }

    std::stringstream ss;
    if(!prim->postponeLinking()) {
        // if primary part is properly linked
        if(prim->isReferringToClass()) {
            someType = prim->getReferredClass();
            setMethodForMethodInvokeFromCompilation(invoke, someType, methodSignature, tok);
        } else if(prim->isReferringToField()) {
            type = prim->getReferredField()->getField()->getFieldType();
            setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
        } else if(prim->isReferringToClassMethod()) {
            type =  prim->getReferredClassMethod()->getClassMethod()->getMethodHeader()->getReturnType();
            setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
        } else if(prim->isReferringToInterfaceMethod()) {
            type = prim->getReferredInterfaceMethod()->getInterfaceMethod()->getReturnType();
            setMethodForMethodInvokeFromType(type, invoke, methodSignature, tok);
        } else if(prim->isReferringToConstructor()) {
            // must be NewClassCreation
            someType = ((NewClassCreation*) prim)->getTableOfCreatedClass();
            setMethodForMethodInvokeFromCompilation(invoke, someType, methodSignature, tok);
        } else if(prim->linkToPrimitiveLiteral()) {
            reportLiteralDereferencing((LiteralOrThis*) prim, tok);
        } else if(prim->linkToArray()) {
            // must be PrimaryNewArray
            ss << "Attempting to access method member of an array of '"
               << ((PrimaryNewArray*) prim)->getArrayType()->getTypeAsString() << "' which does not exist.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        } else if(prim->linkToArrayLength()) {
            ss << "Attempting to access method member of type 'int' which does not exist.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        } else if(prim->linkToNull()) {
            reportLiteralDereferencing((LiteralOrThis*) prim, tok);
        } else {
            assert(prim->resolvedLinkButNoEntity());
            // then three cases
            if(prim->isBracketedExpression()) {
                Expression* bracketed = ((BracketedExpression*) prim)->getExpressionInside();
                if(!bracketed->isExprTypeNotEvaluated()) {
                    if(!bracketed->isExprTypeObject()) {
                        ss << "Expecting an expression that evaluated to a class or interface but instead got '"
                           << bracketed->getExpressionTypeString() << "'.";
                        Error(E_DISAMBIGUATION, tok, ss.str());
                    } else {
                        setMethodForMethodInvokeFromCompilation(invoke, bracketed->getTableTypeOfExpression(),
                                        methodSignature, tok);
                    }
                }
            } else if(prim->isQualifiedThis()) {
                // QualifiedThis
                setMethodForMethodInvokeFromCompilation(invoke, curCompilation, methodSignature, tok);;
            } else {
                // Array Access
                assert(prim->isArrayAccessName() || prim->isArrayAccessPrimary());
                ArrayAccess* accessPrim = (ArrayAccess*) prim;
                someType = accessPrim->getTableOfArrayObjects();
                if(someType != NULL) {
                    setMethodForMethodInvokeFromCompilation(invoke, someType, methodSignature, tok);
                } else {
                    // since someType == NULL, it must mean that the accessed array element
                    // is not a class or interface type
                    ss << "Expecting a class or interface but instead got '"
                       << getCorrespondingTypeString(accessPrim->getTypeOfArrayElements(), accessPrim->getTableOfArrayObjects())
                       << "'.";
                    Error(E_DISAMBIGUATION, tok, ss.str());
                }
            }
        }
    }
}

void AmbiguousLinker::traverseAndLink(ArrayAccess* access) {
    if(access->isArrayAccessName()) {
        traverseAndLink((ArrayAccessName*) access);
    } else {
        traverseAndLink((ArrayAccessPrimary*) access);
    }

    traverseAndLink(access->getAccessExpression());
}

void AmbiguousLinker::traverseAndLink(ArrayAccessName* access) {
    Name* name = access->getNameOfAccessedArray();
    Token* tok = name->getNameId()->getToken();
    traverseAndLink(name);
    std::stringstream ss;
    if(!name->postponeLinking()) {
        // if linking went fine
        Type* type = NULL;
        EVALUATED_TYPE accessType = ET_NOTEVALUATED;
        CompilationTable* table = NULL;
        if(name->isReferringToField()) {
            type = name->getReferredField()->getField()->getFieldType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type); 
        } else if(name->isReferringToLocalVar()) {
            type = name->getReferredLocalVar()->getLocalDecl()->getLocalType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
        } else if(name->isReferringToParameter()) {
            type = name->getReferredParameter()->getParameter()->getParameterType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
        } else {
            // Everything else is wrong
            ss << "Expected an array but instead found ";
            if(name->isReferringToType()) {
                if(name->getReferredType()->isClassSymbolTable()) {
                    ss << " the name of a class.";
                } else { ss << " the name of an interface."; }
            } else if(name->isReferringToPackage()) {
                ss << " the name of a package.";
            } else {
                assert(name->linkToArrayLength());
                ss << " an integer.";
            }
            Error(E_DISAMBIGUATION, tok, ss.str());
            return;
        }

        if(accessType == ET_OBJECTARRAY || accessType == ET_OBJECT) {
            table = ((ReferenceType*) type)->getReferenceTypeTable();
        }

        if(checkProperArrayAccess(access, table, accessType, tok)) {
            // everything went fine and dandy
            access->ResolveLinkButNoEntity();
        }
    }
}

void AmbiguousLinker::traverseAndLink(ArrayAccessPrimary* access) {
    Primary* prim = access->getAccessedPrimaryArray();
    traverseAndLink(access->getAccessedPrimaryArray());
    Token* tok = getTokenFromPrimary(prim);

    if(!prim->postponeLinking()) {
        // if linking went fine
        Type* type = NULL;
        CompilationTable* table = NULL;
        EVALUATED_TYPE accessType = ET_NOTEVALUATED;
        if(prim->isReferringToField()) {
            type = prim->getReferredField()->getField()->getFieldType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type); 
        } else if(prim->isReferringToClassMethod()) {
            type = prim->getReferredClassMethod()->getClassMethod()->getMethodHeader()->getReturnType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
        } else if(prim->isReferringToInterfaceMethod()) {
            type = prim->getReferredInterfaceMethod()->getInterfaceMethod()->getReturnType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
        } else if(prim->linkToArray()) {
            // must be PrimaryNewArray
            type = ((PrimaryNewArray*) prim)->getArrayType();
            accessType = (EVALUATED_TYPE) returnEvalTypeFromTypeNode(type);
            if(accessType != ET_OBJECT) {
                // anything else whose type is not ET_OBJECT then +5
                // in this case accessType can only be ET_INT, ET_CHAR, ET_BOOLEAN,
                // ET_SHORT, ET_BYTE
                // precautionary check
                assert(accessType != ET_VOID && accessType != ET_NULL &&
                       accessType != ET_NOTEVALUATED && accessType != ET_OBJECTARRAY);
                accessType = (EVALUATED_TYPE) (accessType + 5);
            } else { accessType = ET_OBJECTARRAY; }
        } else {
            // everything else is error
            std::stringstream ss;
            if(prim->isReferringToConstructor()) {
                // then it's NewClassCreation
                ss << "Expected an array but instead got a class instance creation expression.";
            } else if(prim->linkToPrimitiveLiteral() || prim->linkToNull()) {
                // must be LiteralOrThis
                ss << "Expected an array but instead got literal '"
                   << ((LiteralOrThis*) prim)->getLiteralAsString() << "'.";
            } else if(prim->linkToArrayLength()) {
                // must be a FieldAccess
                ss << "Expected an array but instead got an int.";
            } else {
                assert(prim->resolvedLinkButNoEntity());
                if(prim->isBracketedExpression()) {
                    accessType = ((BracketedExpression*) prim)->getExpressionInside()->getExprType();
                    table = ((BracketedExpression*) prim)->getExpressionInside()->getTableTypeOfExpression();
                    if(accessType != ET_NOTEVALUATED) {
                        if(checkProperArrayAccess(access, table, accessType, tok)) {
                            // all fine and dandy
                            access->resolvedLinkButNoEntity();
                        }
                    }
                    return;
                } else if(prim->isQualifiedThis()) {
                    ss << "Expected an array but instead got qualified this which refers to '"
                       << ((QualifiedThis*) prim)->getQualifyingClassTable() << "'.";
                } else {
                    assert(prim->isArrayAccessName() || prim->isArrayAccessPrimary());
                    ArrayAccess* accessPrim = (ArrayAccess*) prim;
                    ss << "Expected an array but instead got '"
                       << getCorrespondingTypeString(accessPrim->getTypeOfArrayElements(), accessPrim->getTableOfArrayObjects())
                       << "'.";
                }
                Error(E_DISAMBIGUATION, tok, ss.str());
            }
        }
        
        if(accessType == ET_OBJECTARRAY || accessType == ET_OBJECT) {
            table = ((ReferenceType*) type)->getReferenceTypeTable();
        }

        if(checkProperArrayAccess(access, table, accessType, tok)) {
            // everything went fine and dandy
            access->ResolveLinkButNoEntity();
        }
    }
}

void AmbiguousLinker::traverseAndLink(AssignArray* assign) {
    std::stringstream ss;
    ArrayAccess* assignArray = assign->getAssignedArray();
    traverseAndLink(assignArray);
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

void AmbiguousLinker::traverseAndLink(CastExpression* cast) {
    if(cast->isCastToArrayName() || cast->isCastToReferenceType()) {
        traverseAndLink((CastName*) cast);
    } else {
        // cast to primitve array
        traverseAndLink((CastPrimitive*) cast);
    }
    traverseAndLink(cast->getExpressionToCast());
}

void AmbiguousLinker::traverseAndLink(CastName* cast) {
    // commented out for now, since technically it's not needed
    /* traverseAndLink(nameToCastTo);
    // an error in linking name, quietly return
    if(nameToCastTo->postponeLinking()) { return; }
    */

    // name must necesserarily now link to some class/interface
    // due to A2's check
    cast->setExprType(ET_OBJECT, cast->getCastTypeTable());

    // precautionary check
    if(cast->isCastToArrayName()) {
        // reconfigure if this is a cast to an array of class
        // or interface
        cast->reconfigureExprType(ET_OBJECTARRAY);
    }    
}

void AmbiguousLinker::traverseAndLink(CastPrimitive* cast) {
    PrimitiveType* type = cast->getPrimitiveTypeToCastTo();
    setExpressionTypeBasedOnType(cast, type);
}

void AmbiguousLinker::traverseAndLink(ClassMethod* method) {
    // update symbol table to point to class method
    curSymTable = method->getClassMethodTable();
    FormalParamStar* params = method->getMethodHeader()->getClassMethodParams();
    if(!params->isEpsilon()) {
        // update symbol table to point to the right most parameter, if there's any
        curSymTable = params->getListOfParameters()->getParamTable();
    }

    traverseAndLink(method->getMethodBody());
    // reset to class method table again, in case it was change while
    // traversing the body
    curSymTable = method->getClassMethodTable();
}

void AmbiguousLinker::traverseAndLink(MethodBody* body) {
    if(!body->noDefinition()) {
        traverseAndLink(body->getBlockStmtsStar());
    }
}

void AmbiguousLinker::traverseAndLink(BlockStmtsStar* block) {
    if(!block->isEpsilon()) {
        traverseAndLink(block->getStatements());
    }
}

void AmbiguousLinker::traverseAndLink(BlockStmts* stmts) {
    if(!stmts->isLastStatement()) {
        traverseAndLink(stmts->getNextBlockStmt());
    }

    if(stmts->isLocalVarDecl()) {
        traverseAndLink((LocalDecl*) stmts);
    } else if(stmts->isReturnStmt()) {
        traverseAndLink(((ReturnStmt*) stmts)->getReturnExpr());
    } else if(stmts->isAssignStmt() || stmts->isClassCreationStmt() || stmts->isMethodInvokeStmt()) {
        traverseAndLink((StmtExpr*) stmts);
    } else if(stmts->isIfStmt() || stmts->isIfThenElseStmt()) {
        traverseAndLink((IfStmt*) stmts);
    } else if(stmts->isWhileStmt()) {
        traverseAndLink((WhileStmt*) stmts);
    } else if(stmts->isForStmt()) {
        traverseAndLink((ForStmt*) stmts);
    } else if(stmts->isNestedBlock()) {
        traverseAndLink((NestedBlock*) stmts);
    }
}

void AmbiguousLinker::traverseAndLink(LocalDecl* local) {
    // update symbol table (needed for A4)
    curSymTable = local->getLocalTable();
    // set context
    asgmtLocalContext = true;
    assigningLocalVar = local->getLocalId()->getIdAsString();
    traverseAndLink(local->getLocalInitExpr());
    // reset
    asgmtLocalContext = false;
}

void AmbiguousLinker::traverseAndLink(ExpressionStar* exprStar) {
    if(!exprStar->isEpsilon()) {
        traverseAndLink(exprStar->getExpression());
    }
}

void AmbiguousLinker::traverseAndLink(StmtExpr* stmt) {
    if(stmt->isAssignStmt()) {
        traverseAndLink(((StmtExprAssign*) stmt)->getAssignmentExpression());
    } else if(stmt->isClassCreationStmt()) {
        traverseAndLink(((StmtExprCreation*) stmt)->getCreatedClass());
    } else {
        // a method invocation statement
        traverseAndLink(((StmtExprInvoke*) stmt)->getMethodInvoked());
    }
}

void AmbiguousLinker::traverseAndLink(IfStmt* stmt) {
    traverseAndLink(stmt->getExpressionToEvaluate());
    traverseAndLink(stmt->getExecuteTruePart());
    if(!stmt->noElsePart()) {
        traverseAndLink(stmt->getExecuteFalsePart());
    }
}

void AmbiguousLinker::traverseAndLink(WhileStmt* stmt) {
    traverseAndLink(stmt->getExpressionToEvaluate());
    traverseAndLink(stmt->getLoopStmt());
}

void AmbiguousLinker::traverseAndLink(ForStmt* stmt) {
    ForTable* table = stmt->getForTable();
    curSymTable = table;
    LocalTable* localForInit = table->getForInitTable();
    if(localForInit != NULL) {
        // if it actually was a local variable declaration in
        // the ForInit, update the current symbol table
        curSymTable = localForInit;
    }
    if(!stmt->emptyForInit()) {
        traverseAndLink(stmt->getForInit());
    }
    traverseAndLink(stmt->getExpressionToEvaluate());
    
    if(!stmt->emptyForUpdate()) {
        traverseAndLink(stmt->getForUpdate());
    }

    traverseAndLink(stmt->getLoopStmt());
    // reset the symbol table in case
    // it was assigned to something else during the traversing
    curSymTable = table;
}

void AmbiguousLinker::traverseAndLink(NestedBlock* nested) {
    curSymTable = nested->getNestedBlockTable();
    traverseAndLink(nested->getNestedBlock());
    // reset
    curSymTable = nested->getNestedBlockTable();
}

void AmbiguousLinker::traverseAndLink(Constructor* ctor) {
    // update the table to constructor table
    curSymTable = ctor->getConstructorTable();
    FormalParamStar* params = ctor->getConstructorParameters();
    if(!params->isEpsilon()) {
        // update the symbol table to the right most parameter
        curSymTable = params->getListOfParameters()->getParamTable();
    }
    traverseAndLink(ctor->getConstructorBody());
    // reset the symbol table to the constructor table, in case
    // it was changed when traversing the constructor's body
    curSymTable = ctor->getConstructorTable();
}

// ----------------------------------------------------------------------------
        
void AmbiguousLinker::traverseAndLink(CompilationTable* compilation) {
    curCompilation = compilation;
    curSymTable = compilation->getSymbolTable();
    if(curSymTable == NULL) {
        // just return since there is nothing to do
        return;
    }

    if(curSymTable->isClassTable()) {
        // only link for classes, because there's really nothing to do
        // in interfaces
        traverseAndLink(((ClassTable*) curSymTable)->getClass());
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

void AmbiguousLinker::reportIncompletePackageNameUsage(const std::string& pkgName, Token* tok) {
    std::stringstream ss;
    ss << "Incomplete use of package name '" << pkgName
    << "'. Were you trying to access a type from that package?";
    Error(E_DISAMBIGUATION, tok, ss.str());
}

void AmbiguousLinker::reportLiteralDereferencing(LiteralOrThis* lit, Token* tok) {
    std::stringstream ss;
    if(lit->isNumber()) {
        ss << "Literal '" << lit->getLiteralAsString() << "' of primitive type integer cannot be dereferenced.";
    } else if(lit->isTrueBoolean() || lit->isFalseBoolean()) {
        ss << "Literal '" << lit->getLiteralAsString() << "' of primitive type boolean cannot be dereferenced.";
    } else if(lit->isCharLiteral()) {
        ss << "Literal '" << lit->getLiteralAsString() << "' of primitive type char cannot be dereferenced.";
    } else if(lit->isNull()) {
        ss << "Literal 'null' cannot be dereferenced.";
    }
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
        ss << "Attempting to access static member of interface '" << fullName << "'.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        return false;
    }
    return true;
}

FieldTable* AmbiguousLinker::findFieldDeclaredInClass(const std::string& fieldName, bool previous) {
    // search for Field in own class BUT not including inherited fields
    SymbolTable* symTable = curSymTable;
    while(symTable != NULL) {
        if(symTable->isFieldTable()) {
            if(((FieldTable*) symTable)->getField()->getFieldDeclared()->getIdAsString() == fieldName) {
                return (FieldTable*) symTable;
            }
        }
        if(previous) {
            // search mode is backwards
            symTable = symTable->getPrevTable();
        } else {
            // search mode is forwards
            symTable = symTable->getNextTable();
        }
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
            // then this should be a type, assumption is that packages and types don't
            // clash, as it was checked in A2
            CompilationTable* typeTable = pkg->getACompilationWithName(currName);
            if(typeTable == NULL) {
                // if it's still NULL, then error
                ss << "No subpackage or type '" << currName << "' can be identified via package '"
                   << nextName->getFullName() << "'.";
                Error(E_DISAMBIGUATION, tok, ss.str());
            } else {
                // referring to some type
                name->setReferredType(typeTable);
            }
        }
    } else if(nextName->isReferringToType()) {
        CompilationTable* someType = nextName->getReferredType();
        // check that typeTable refers to a class
        if(checkTypeIsClassDuringStaticAccess(someType, currName, tok)) {
            // it passed therefore it is a class, can safely do this
            FieldTable* fieldFound = getFieldInAClass(someType, currName, tok);
            if(fieldFound != NULL) {
                // field was found
                name->setReferredField(fieldFound);
            } else {
                std::stringstream ss;
                ss << "Unable to locate field '" << currName << "' in class '"
                   << someType->getCanonicalName() << "'.";
                Error(E_DISAMBIGUATION, tok, ss.str());
            }
        }
        // the rest are what is categorized in the JLS as ExpressionName
    } else if(nextName->isReferringToField()) {
        FieldDecl* field = nextName->getReferredField()->getField();
        if(checkProperMemberAccessingFromVariable(currName, field->getFieldType(), tok)) {
            if(!setNameReferringToArrayLength(name, field->getFieldType())) {
                // name wasn't linked to array length, when we get here we know
                // that FieldDecl's type must be a ReferenceType to some class, and not
                // an array nor interface
                linkNameToFieldFromType(name, (ReferenceType*) field->getFieldType(), currName);
            }
        }
    } else if(nextName->isReferringToLocalVar()) {
        LocalDecl* localVar = nextName->getReferredLocalVar()->getLocalDecl();
        if(checkProperMemberAccessingFromVariable(currName, localVar->getLocalType(), tok)) {
            if(!setNameReferringToArrayLength(name, localVar->getLocalType())) {
                // same thing as above
                linkNameToFieldFromType(name, (ReferenceType*) localVar->getLocalType(), currName);
            }
        }
    } else if(nextName->isReferringToParameter()) {
        ParamList* param = nextName->getReferredParameter()->getParameter();
        if(checkProperMemberAccessingFromVariable(currName, param->getParameterType(), tok)) {
            if(!setNameReferringToArrayLength(name, param->getParameterType())) {
                // same thing as above
                linkNameToFieldFromType(name, (ReferenceType*) param->getParameterType(), currName);
            }
        }
    } else {
        // atempting to access array length?
        assert(name->linkToArrayLength());
        ss << "Primitive type int cannot be dereferenced.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    }
}

void AmbiguousLinker::linkSimpleName(Name* name) {
    // pretty much a simple name, then do these checks
    // Order based on JLS 6.5.2
    
    Token* tok = name->getNameId()->getToken();
    std::string currName = tok->getString();
    if(withinMethod) {
        // the check is occurring inside a method
        SymbolTable* localOrParam = findLocalVarOrParameterPreviouslyDeclared(currName);
        if(localOrParam != NULL) {
            // local variable or parameter was found
            if(asgmtLocalContext) {
                // within the context of the initialization of a local variable
                std::stringstream ss;
                if(localOrParam->isLocalTable()) {
                    if(assigningLocalVar == ((LocalTable*) localOrParam)->getLocalDecl()->getLocalId()->getIdAsString()) {
                        // usage of local variable in its own initializer, assumption is that
                        // local variables are unique within its scope, checked at symbol table stage
                        ss << "Use of local variable '" << assigningLocalVar << "' in its own initializer.";
                        Error(E_DISAMBIGUATION, tok, ss.str());
                    }
                }
            }

            if(localOrParam->isLocalTable()) {
                name->setReferredLocalVar((LocalTable*) localOrParam);
            } else { 
                // not a local variable, but a parameter
                name->setReferredParameter((ParamTable*) localOrParam);
            }
            return;
        } else {
            // name isn't referring to a local variable or parameter, search it as a field
            // of this class, any field even if it was declared after the method being checked
            FieldTable* field = curCompilation->getAField(currName);
            if(field != NULL) {
                // the field was found
                name->setReferredField(field);
                return;
            }
        }
    } else {
        FieldTable* field = findFieldDeclaredInClass(currName, true);
        if(field != NULL) {
            // the field was found
            name->setReferredField(field);
            return;
        }
        // field couldn't be gotten from previous declaration (not including self)
        if(asgmtFieldContext) {
            // check if we are in the context of an assignment in a FieldDecl initializer
            // if we are then try to grab the field anywhere from the class, so now check for
            // fields defined later
            field = findFieldDeclaredInClass(currName, false);
            if(field != NULL) {
                // the field was found
                name->setReferredField(field);
                return;
            }
        }

        // all in all if we still can't find it then search for inherited ones
        field = curCompilation->getAField(currName);
        if(field != NULL) {
            if(!asgmtFieldContext) {
                if(field == findFieldDeclaredInClass(currName, false)) {
                    // if the field actually found is a field later on declared
                    // AND we are not in a context of an assignment in a FieldDeclaration
                    std::stringstream ss;
                    ss << "Simple name '" << currName << "' refers to a field declared later on "
                       << "in the class and is used not in the left-hand side of an assignment in an initialization of a "
                       << "field declaration.";
                    Error(E_DISAMBIGUATION, tok, ss.str());
                    return;
                }
            }
            // all is good
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
    
    typeTable = curCompilation->checkTypePresenceFromImportOnDemand(currName, tok);
    if(typeTable != NULL) {
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
    ss << "Simple name '" << currName
       << "' does not refer to either a local variable, parameter, field, type nor package in the environment previously declared.";
    Error(E_DISAMBIGUATION, tok, ss.str());
}

FieldTable* AmbiguousLinker::getFieldInAClass(CompilationTable* someClass, const std::string& findField, Token* tok) {
    FieldTable* field = someClass->getAField(findField);
    if(field == NULL) {
        // if the field cannot be found
        std::stringstream ss;
        ss << "Unable to locate field '" << findField << "' in class '" << someClass->getCanonicalName() << "'.";
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
        // if it is, then we know that the accessed member MUST be length, because
        // this is only called when the check of checkProperMemberAccessingFromVariable
        // passed
        name->setReferringToArrayLength();
        return true;
    }
    return false;
}

bool AmbiguousLinker::setFieldAccessReferringToArrayLength(FieldAccess* access, Type* type) {
    if((type->isPrimitiveType() && ((PrimitiveType*) type)->isPrimitiveArray())
       || type->isReferenceArrayType()) {
        // either a primitive array or an array of some class/interface, same case as above
        access->setReferringToArrayLength();
        return true;
    }
    return false;
}

int AmbiguousLinker::returnEvalTypeFromTypeNode(Type* type) {
    if(type == NULL) {
        // in the case that it is a void return type of some method
        // the only possible case theoretically speaking
        return ET_VOID;
    }
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
        // since this function is called by either:
        // - traverseAndLink(Expression*) where Expression is a NameExpression
        // - traverseAndLink(AssignName*)
        // - traverseAndLink(AssignArray*) where the array being assigned is in the form of a Name
        // then this CAN'T be a type, no way hosay
        ss << "Invalid use of name '" << name->getFullName() << "' that refers to a ";
        if(name->getReferredType()->isClassSymbolTable()) {
            ss << "class.";
        } else {
            ss << "interface.";
        }
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
        reportIncompletePackageNameUsage(name->getFullName(), tok);
        return;
    }

    setExpressionTypeBasedOnType(expr, type);
}

void AmbiguousLinker::checkProperArrayAccessInExpression(Expression* expr, Token* tok) {
    // check if the expression type is set, if not
    // then there must have been an error before -> silently return
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
    } else if(expr->isExprTypeVoid()) {
        ss << "void.";
        Error(E_DISAMBIGUATION, tok, ss.str());
    } else if(expr->isExprTypeNull()) {
        ss << "null.";
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
        // either QualifiedThis, BracketedExpression or ArrayAccess
        if(prim->isBracketedExpression()) {
            Expression* bracketed = ((BracketedExpression*) prim)->getExpressionInside();
            // just copy the type
            expr->setExprType(bracketed->getExprType(), bracketed->getTableTypeOfExpression());
            // getting a Token from an expression is too bothersome lol, maybe some other time
        } else if(prim->isQualifiedThis()) {
            // QualifiedThis
            expr->setExprType(ET_OBJECT, ((QualifiedThis*) prim)->getQualifyingClassTable());
            tok = ((QualifiedThis*) prim)->getQualifyingClassName()->getNameId()->getToken();
        } else {
            // ArrayAccess
            assert(prim->isArrayAccessName() || prim->isArrayAccessPrimary());
            expr->setExprType(((ArrayAccess*) prim)->getTypeOfArrayElements(), ((ArrayAccess*) prim)->getTableOfArrayObjects());
        }
    } else if(prim->isReferringToClass()) {
        expr->setExprType(ET_OBJECT, prim->getReferredClass());
    } else {
        // the only one remaining, precautionary check
        assert(prim->linkToArrayLength());
        assert(prim->linkToArrayLength() && prim->isFieldAccess());
        expr->setExprType(ET_INT);
        tok = ((FieldAccess*) prim)->getAccessedFieldId()->getToken();
    }

    return tok;
}

SymbolTable* AmbiguousLinker::findLocalVarOrParameterPreviouslyDeclared(const std::string& currName) {
    SymbolTable* table = curSymTable;
    while(!table->isClassMethodTable() && !table->isConstructorTable()) {
        // while the symbol table isn't referring to the class method or constructor
        if(table->isLocalTable()) {
            if(((LocalTable*) table)->getLocalDecl()->getLocalId()->getIdAsString() == currName) {
                // got the local variable
                return table;
            }
        } else if(table->isParamTable()) {
            if(((ParamTable*) table)->getParameter()->getParameterId()->getIdAsString() == currName) {
                // got the parameter
                return table;
            }
        }
        table = table->getPrevTable();
    }
    return NULL;
}

void AmbiguousLinker::setMethodForMethodInvokeFromCompilation(MethodInvoke* invoke, CompilationTable* someType,
                    const std::string& methodSignature, Token* tok) {
    std::stringstream ss;
    if(someType->isClassSymbolTable()) {
        ClassMethodTable* method = someType->getAClassMethod(methodSignature);
        if(method != NULL) {
            invoke->setReferredClassMethod(method);
        } else {
            /* InterfaceMethodTable* method = someType->getInterfaceMethodFromClass(methodSignature);
            if(method != NULL) {
                invoke->setReferredInterfaceMethod(method);
                return;
            }*/
            // anything else
            ss << "Class method with signature '" << methodSignature << "' could not be found in class '"
               << someType->getCanonicalName() << "'.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        }
    } else {
        // an interface
        InterfaceMethodTable* method = someType->getAnInterfaceMethod(methodSignature);
        if(method != NULL) {
            invoke->setReferredInterfaceMethod(method);
        } else {
            // then try from objects
            ClassMethodTable* objectMethod = someType->getAnInterfaceMethodFromObject(methodSignature);
            if(objectMethod != NULL) {
                invoke->setReferredClassMethod(objectMethod);
                return;
            }
            // else error
            ss << "Interface method with signature '" << methodSignature << "' could not be found in interface '"
               << someType->getCanonicalName() << "'.";
            Error(E_DISAMBIGUATION, tok, ss.str());
        }
    }
}

void AmbiguousLinker::setMethodForMethodInvokeFromType(Type* type, MethodInvoke* invoke,
                const std::string& methodSignature, Token* tok) {
    if(type->isClassOrInterfaceType()) {
        CompilationTable* someType = ((ReferenceType*) type)->getReferenceTypeTable();
        setMethodForMethodInvokeFromCompilation(invoke, someType, methodSignature, tok);
    } else {
        std::stringstream ss;
        ss << "Attempting to access method member of type '" << type->getTypeAsString() << "' which does not exist";
        Error(E_DISAMBIGUATION, tok, ss.str());
    }
}

bool AmbiguousLinker::checkProperMethodOrConstructorSignature(const std::string& methodSignature, Token* tok) {
    bool proper = true;
    std::stringstream ss;
    if(methodSignature.find("null") != std::string::npos) {
        // it was found
        ss << "Invalid use of null literal in method invocation. Please cast it to an appropriate type first.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        proper = false;
    } else if(methodSignature.find("void") != std::string::npos) {
        ss << "Invalid use of void as one of the arguments given to the method.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        proper = false;
    } else if(methodSignature.find("N/A") != std::string::npos) {
        // one of the argument's type cannot be evaluated, a previous error
        // occurred
        proper = false;
    }
    return proper;
}

bool AmbiguousLinker::checkProperArrayAccess(ArrayAccess* access, CompilationTable* table, EVALUATED_TYPE accessType, Token* tok) {
    std::stringstream ss;
    if(accessType == ET_INTARRAY || accessType == ET_CHARARRAY || accessType == ET_BYTEARRAY
       || accessType == ET_SHORTARRAY || accessType == ET_BOOLEANARRAY) {
        // turn to non-array type
        access->setTypeOfArrayElements((EVALUATED_TYPE) (accessType-5));
    } else if(accessType == ET_OBJECTARRAY) {
        // then type must have been a reference type
        access->setTypeOfArrayElements(ET_OBJECT, table);
    } else {
        // everything else is an error
        ss << "Expected an array but instead found an entity of type '"
           << getCorrespondingTypeString(accessType, table) << "'.";
        Error(E_DISAMBIGUATION, tok, ss.str());
        return false;
    }

    return true;
}

Token* AmbiguousLinker::getTokenFromPrimary(Primary* prim) {
    Token* tok = NULL;
    if(prim->isQualifiedThis()) {
        tok = ((QualifiedThis*) prim)->getQualifyingClassName()->getNameId()->getToken();
    } else if(prim->isArrayAccessName()) {
        tok = ((ArrayAccessName*) prim)->getNameOfAccessedArray()->getNameId()->getToken();
    } else if(prim->isArrayAccessPrimary()) {
        tok = getTokenFromPrimary(((ArrayAccessPrimary*) prim)->getAccessedPrimaryArray());
    } else if(prim->isFieldAccess()) {
        tok = ((FieldAccess*) prim)->getAccessedFieldId()->getToken();
    } else if(prim->isBracketedExpression()) {
        // too lazy to do this
        tok = NULL;
    } else if(prim->isNormalMethodCall()) {
        tok = ((MethodNormalInvoke*) prim)->getNameOfInvokedMethod()->getNameId()->getToken();
    } else if(prim->isAccessedMethodCall()) {
        tok = ((InvokeAccessedMethod*) prim)->getAccessedMethod()->getAccessedFieldId()->getToken();
    } else if(prim->isNewClassCreation()) {
        tok = ((NewClassCreation*) prim)->getClassName()->getNameId()->getToken();
    } else if(prim->isNewPrimitiveArray()) {
        tok = ((PrimitiveType*) ((PrimaryNewArray*) prim)->getArrayType())->getPrimTypeToken();
    } else if(prim->isNewReferenceArray()) {
        tok = ((ReferenceType*) ((PrimaryNewArray*) prim)->getArrayType())->getReferenceName()->getNameId()->getToken();
    } else {
        // the remaining is LiteralOrThis
        tok = ((LiteralOrThis*) prim)->getLiteralToken();
    }
    return tok;
}

void AmbiguousLinker::linkNameToFieldFromType(Name* name, ReferenceType* type, const std::string& fieldName) {
    // no error checking needs to be done since they've all been done
    // in checkProperMemberAccessingFromVariable and in setNameReferringToArrayLength
    // and we've passed those stages by the time this is called
    CompilationTable* table = type->getReferenceTypeTable();
    FieldTable* field = getFieldInAClass(table, fieldName, name->getNameId()->getToken());
    if(field != NULL) {
        name->setReferredField(field);
    }
}

void AmbiguousLinker::linkFieldAccessFromType(FieldAccess* access, ReferenceType* type, const std::string& fieldName) {
    // same as above, except this is for FieldAccess
    CompilationTable* table = type->getReferenceTypeTable();
    FieldTable* field = getFieldInAClass(table, fieldName, access->getAccessedFieldId()->getToken());
    if(field != NULL) {
        access->setReferredField(field);
    }
}

void AmbiguousLinker::linkFieldAccessFromCompilation(FieldAccess* access, CompilationTable* someClass,
                const std::string& fieldName, Token* tok) {
    FieldTable* fieldGotten = getFieldInAClass(someClass, fieldName, tok);
    if(fieldGotten != NULL) {
        access->setReferredField(fieldGotten);
    }
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
