#include <cassert>
#include <iostream>
#include <queue>
#include <algorithm>

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
#include "referenceType.h"
#include "interfaceMethod.h"

// Symbol table
#include "compilationTable.h"
#include "classTable.h"
#include "paramTable.h"
#include "classMethodTable.h"
#include "interfaceMethodTable.h"

// Code generation related
#include "codeGenerator.h"
#include "startup.h"
// Managers
#include "vtableManager.h"
#include "objectLayoutManager.h"
#include "inheritanceTableManager.h"
#include "implementedInterfaceMethodTableManager.h"
#include "staticFieldsManager.h"

// Tables and layouts
#include "vtableLayout.h"
#include "objectLayout.h"
#include "implementedInterfaceMethodTable.h"
#include "inheritanceTable.h"
#include "staticFields.h"

// Label
#include "labelManager.h"
#include "RLG.h"

// ------------------------------------------------------------------------------------
// Helper

void CodeGenerator::CALL_FUNCTION(std::string fn_name) {
    std::string constructor_prefix = LabelManager::labelizeForConstructor(processing->getCanonicalName()) + "$";
    std::string method_prefix = processing->getCanonicalName() + "$";
    std::string alloc_prefix = LabelManager::labelizeForAlloc(processing->getCanonicalName());

    // If it is a constructor we need to allocate 'this'
    if (fn_name.find(LabelManager::getConstructor()) == 0) {
        // Get canonincal name of type constructor being invoked
        std::string canonical_name = fn_name.substr(LabelManager::getConstructor().size());
        canonical_name = canonical_name.substr(0, canonical_name.find("$"));

        CALL_FUNCTION(LabelManager::labelizeForAlloc(canonical_name));
        asma("push eax ; push this");
    }

    // Check if we need to extern the constructor
    if (fn_name.find(constructor_prefix) != 0 &&
               fn_name.find(method_prefix) != 0 &&
               (fn_name != alloc_prefix)) {
        asma("extern " << fn_name);   
    }
    asma("call " << fn_name);
}

void CodeGenerator::CALL_IDIOM() {
    asma("push ebp");
    asma("mov ebp, esp");
}

void CodeGenerator::RETURN_IDIOM() {
    asma("mov esp, ebp");
    asma("pop ebp");
    asma("ret");
}

SymbolTable* CodeGenerator::getParamOrLocalTableForName(Name* name) {
    if (name->isReferringToParameter()) {
        return name->getReferredParameter();
    } else if (name->isReferringToLocalVar()) {
        return name->getReferredLocalVar();
    } else {
        assert(false);
    }

    return NULL;
}

void CodeGenerator::setParameterOffsetFromEBP(ParamList* params, int start_offset) {
    // Param List is right to left, but we need to increment by left to right
    // So flip it
    std::queue<ParamList*> paramQueue;
    unsigned int param_offset = start_offset;
    while (params != NULL) {
        paramQueue.push(params);
        params = params->getNextParameter();
    }

    while (!paramQueue.empty()) {
        params = paramQueue.front();
        paramQueue.pop();

        addressTable[params->getParamTable()] = param_offset;
        param_offset += 4;
    }
}

void CodeGenerator::callInitializersOfDeclaredFields() {
    // Call fields initializers here
    SymbolTable* symTable = processing->getSymbolTable()->getNextTable();
    while(symTable != NULL) {
        if(symTable->isFieldTable()) {
            if(!((FieldTable*) symTable)->getField()->isStatic()) {
                // a non-static field, call it's initializer
                asma("call " << (((FieldTable*) symTable)->generateFieldInitializerLabel()));
            }
        }
        symTable = symTable->getNextTable();
    }
}

void CodeGenerator::exceptionCall() {
    asma("extern __exception");
    asma("call __exception");
}

void CodeGenerator::arrayCreationCall(const std::string& size) {
    asma("mov eax, " << size);
    CALL_FUNCTION("makeArrayBanana$");
}

void CodeGenerator::primitiveStringConversion(const std::string& wrapperType, const std::string& primitiveType) {
    asmc("CONCAT string with " << primitiveType << " primitive");
    // Save eax to prevent thrashing
    asma("push eax");
    // Push integer for constructor parameter
    asma("push ebx");
    // Call Constructor of the wrapper type
    CALL_FUNCTION(LabelManager::labelizeForConstructor("java.lang." + wrapperType, 1, primitiveType.c_str()));

    CALL_FUNCTION("java.lang." + wrapperType + ".toString$$");
    asma("pop ebx ; pop created " << wrapperType);
    asma("pop ebx ; pop old ebx");
    asma("mov ebx, eax ; move created String to ebx");
    asma("pop eax ; pop old eax");
}

// -------------------------------------------------------------------------------------
// Real deal

CodeGenerator::CodeGenerator(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) :
    compilations(compilations),
    starter(new Startup(compilations, firstUnit)),
    virtualManager(new VTableManager(compilations)),
    objManager(new ObjectLayoutManager(compilations)),
    inhManager(new InheritanceTableManager(compilations)),
    interManager(new ImplInterfaceMethodTableManager(compilations)),
    staticManager(new StaticFieldsManager(compilations)),
    fs(NULL),
    scope_offset(0)
{}

CodeGenerator::~CodeGenerator() {
    delete starter;
    delete virtualManager;
    delete objManager;
    delete inhManager;
    delete interManager;
    delete staticManager;
}

void CodeGenerator::generateStartFile() {
    // generate _startup.s
    std::vector<InheritanceTable*> arrayInheritance;
    std::vector<StaticFields*> statics;
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined()) {
            // for each type defined, get their corresponding array type
            arrayInheritance.push_back(inhManager->getTableForType(it->second->getCanonicalName() + "[]"));
            
            if(it->second->isClassSymbolTable()) {
                // a class, grab all it's static methods
                statics.push_back(staticManager->getStaticFieldsForClass(it->second));
            }
        }
    }
    // get the array types for primitive types
    arrayInheritance.push_back(inhManager->getTableForType("int[]"));
    arrayInheritance.push_back(inhManager->getTableForType("short[]"));
    arrayInheritance.push_back(inhManager->getTableForType("byte[]"));
    arrayInheritance.push_back(inhManager->getTableForType("char[]"));
    arrayInheritance.push_back(inhManager->getTableForType("boolean[]"));

    starter->generateStartupFile(virtualManager->getVTableLayoutForArray(), arrayInheritance,
                interManager->getTableForArray(), statics);
}

// --------------------------------------------------------------------------------
// Code generation section

void CodeGenerator::traverseAndGenerate() {
    std::map<std::string, CompilationTable*>::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        if(it->second->aTypeWasDefined() && it->second->isClassSymbolTable()) {
            // a type was defined and it's a class
            std::string classCanonicalName = it->second->getCanonicalName();
#if defined(CODE_OUT)
            std::stringstream ss;
            ss << CODE_OUT << "/" << classCanonicalName << ".s";
            fs = new std::ofstream(ss.str());
#else
            fs = new std::ofstream(classCanonicalName + ".s");
#endif
            processing = it->second;
            bool isAbstractClass = ((ClassTable*) processing->getSymbolTable())->getClass()->isAbstract();

            // data section
            section("data");

            // expose all static fields declared within the class
            // initialize all of them to default values
            std::vector<FieldTable*>& staticFields = staticManager->getStaticFieldsForClass(processing)->getAllStaticFieldsOfClass();
            for(unsigned int i = 0; i < staticFields.size(); i++) {
                asmgl(staticFields[i]->generateFieldLabel());
                asma("dd " <<  0);
            }

            if(!isAbstractClass) {
                // Is not an abstract class
                // Make the virtual, inheritance and interface method table for this class
                virtualManager->getVTableLayoutForType(classCanonicalName)->outputVTableToFile(*fs);
                inhManager->getTableForType(classCanonicalName)->outputInheritanceTableToFile(*fs);
                interManager->getTableForType(classCanonicalName)->outputImplInterfaceMethodTableToFile(*fs);
            }

            // text section
            section("text");

            if(!isAbstractClass) {
                // not an abstract class, create allocator for class
                asml(LabelManager::labelizeForAlloc(classCanonicalName));
                asma("mov eax, " << objManager->getLayoutForClass(processing)->sizeOfObject());
                CALL_FUNCTION("__malloc");
                
                asmc("In this order fill in: virtual table, inheritance table, interface methods, type number");
                asma("mov [eax], " << virtualManager->getVTableLayoutForType(classCanonicalName)->getVirtualTableName());
                asma("mov [eax-4], " << inhManager->getTableForType(classCanonicalName)->generateInheritanceTableName());
                asma("mov [eax-8], " << interManager->getTableForType(classCanonicalName)->generateTableName());
                asma("mov [eax-12], " << inhManager->getTypeMapping(classCanonicalName));
                asma("push eax ; push allocated object onto the stack as 'this'");
                
                ObjectLayout* layoutOfClass = objManager->getLayoutForClass(processing);
                asmc("Initialize all fields to their default 0");
                for(unsigned int i = 0; i < layoutOfClass->numberOfFieldsInObject(); i++) {
                    asma("mov [eax - " << ObjectLayout::transformToFieldIndexInAnObject(i) << "], 0");
                }
                asma("ret ; get back to whoever called this allocator, with eax pointing to the new object");
            }
            
            traverseAndGenerate(((ClassTable*)it->second->getSymbolTable())->getClass());
            delete fs;
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
        std::string initializerLabel = field->getFieldTable()->generateFieldInitializerLabel();
        bool isStatic = field->isStatic();

        asmc("Field initializer for " << initializerLabel);
        
        if(isStatic) {
            // static field, initializer needs to be globaled
            asmc("Initializer of static needs to be globalled");
            asmgl(initializerLabel);
        } else {
            asml(initializerLabel);
        }

        unsigned int indexOfField = 0;
        if(!isStatic) {
            // not static field, 
            // then need to have the call idiom
            CALL_IDIOM();
            // non-static get the index of the field in the object
            indexOfField = objManager->getLayoutForClass(processing)->indexOfFieldInObject(field->getFieldTable());
        }

        traverseAndGenerate(field->getInitializingExpression());
        asmc("Initialize field with it's initializer value");
        if(isStatic) {
            asma("mov [" << field->getFieldTable()->generateFieldLabel() << "], eax");
            asma("ret ; just return from static initialize");
        } else {
            asma("mov ebx, [ebp + 8] ; get this");
            asma("mov [ebx - " << indexOfField << "], eax");
            RETURN_IDIOM();
        }
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
    Expression* lhs_expr = binExpr->getLeftExpression();
    Expression* rhs_expr = binExpr->getRightExpression();
    std::string lhs_type = lhs_expr->getExpressionTypeString();
    std::string rhs_type = rhs_expr->getExpressionTypeString();

    // Order based on JLS 15.7.2, except || (lazy or) and && (lazy and)
    traverseAndGenerate(lhs_expr);
    if(!binExpr->isLazyOr() && binExpr->isLazyAnd()) {
        asma("push eax");
        traverseAndGenerate(rhs_expr);
        asma("mov ebx, eax ; put RHS value into ebx");
        asma("pop eax ; put LHS value into eax");
    }

    if(binExpr->isLazyOr()) {
        // Order based on JLS 15.23

        // If LHS is true, then jump
        std::string lbl = LABEL_GEN();
        asmc("Lazy OR expr");
        asma("cmp eax, 1");
        asma("je " << lbl);

        // Else, evaluate RHS
        traverseAndGenerate(rhs_expr);

        asml(lbl);
    } else if(binExpr->isLazyAnd()) {
        // Order based on JLS 15.24
        std::string lbl = LABEL_GEN();

        // If LHS is false, then jump
        asmc("Lazy AND expr");
        asma("cmp eax, 0");
        asma("je " << lbl);

        // Else, evaluate RHS
        traverseAndGenerate(rhs_expr);

        asml(lbl);
    } else if(binExpr->isEagerOr()) {
        // Specific: JLS 15.22.2
        asmc("Eager OR expr");
        asma("or eax, ebx");
    } else if(binExpr->isEagerAnd()) {
        // Specific: JLS 15.22.2
        asmc("Eager AND expr");
        asma("and eax, ebx");
    } else if(binExpr->isEqual()) {
        // Specific: JLS 15.21
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("EQUAL expr");
        asma("cmp eax, ebx");
        // If equal, jump to true and set 1
        asma("je " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isNotEqual()) {
        // Specific: JLS 15.21
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("NOT EQUAL expr");
        asma("cmp eax, ebx");
        // If not equal, jump to true and set 1
        asma("jne " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isLT()) {
        // Specific: JLS 15.20
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("LESS THAN expr");
        asma("cmp eax, ebx");
        // If less, jump to true and set 1
        asma("jl " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isLTE()) {
        // Specific: JLS 15.20 
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("LESS THAN OR EQUAL expr");
        asma("cmp eax, ebx");
        // If less than or equal, jump to true and set 1
        asma("jle " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isGT()) {
        // Specific: JLS 15.20
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("GREATER THAN expr");
        asma("cmp eax, ebx");
        // If greater, jump to true and set 1
        asma("jg " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isGTE()) {
        // Specific: JLS 15.20
        std::string lbl_true = LABEL_GEN();
        std::string lbl_end = LABEL_GEN();

        asmc("GREATER THAN OR EQUAL expr");
        asma("cmp eax, ebx");
        // If greater than or equal, jump to true and set 1
        asma("jge " << lbl_true);
        // Else set 0
        asma("mov eax, 0");
        asma("jmp " << lbl_end);
        
        // Set to 1 since true
        asml(lbl_true);
        asma("mov eax, 1");

        // End
        asml(lbl_end);
    } else if(binExpr->isAddition()) {
        // Specific: JLS 15.18
        if (lhs_type != "java.lang.String" &&
            rhs_type != "java.lang.String") {
            asmc("ADD expr");
            asma("add eax, ebx");
        } else if ((lhs_type == "java.lang.String" && rhs_type != "java.lang.String") ||
                   (lhs_type != "java.lang.String" && rhs_type == "java.lang.String")) {
            // Fake it and swap lhs with rhs
            if (lhs_type != "java.lang.String" && rhs_type == "java.lang.String") {
                Expression* temp = lhs_expr;
                lhs_expr = rhs_expr;
                rhs_expr = temp;

                asmc("Switch around eax and ebx since ebx has the string");
                asma("mov edx, eax ; store lhs temporarily");
                asma("mov eax, ebx ; move rhs value to eax");
                asma("mov ebx, edx ; move lhs value to ebx");
            }

            // Primitive Type
            // - Create object for primitive, then call toString
            if (rhs_expr->isExprTypeInt()) {
                primitiveStringConversion("Integer", "int");
            } else if (rhs_expr->isExprTypeShort()) {
                primitiveStringConversion("Short", "short");
            } else if (rhs_expr->isExprTypeByte()) {
                primitiveStringConversion("Byte", "byte");
            } else if (rhs_expr->isExprTypeChar()) {
                primitiveStringConversion("Character", "char");
            } else if (rhs_expr->isExprTypeBoolean()) {
                primitiveStringConversion("Boolean", "boolean");
            } else {
                // Reference Type and null type
                asmc("CONCAT string with reference type");
                // Save eax to prevent thrashing
                asma("push eax ; save eax");
                // Push object for valueOf parameter
                asma("push ebx ; push ebx as parameter");
                // Call valueOf function in java.lang.String
                CALL_FUNCTION("java.lang.String.valueOf$java.lang.Object$");
                asma("pop ebx ; pop old ebx");
                asma("mov ebx, eax ; move created string to ebx");
                asma("pop eax ; pop old eax");

                // Check if returned string is null
                std::string non_null_lbl = LABEL_GEN();
                asma("cmp ebx, 0");
                asma("jne " <<  non_null_lbl);
                
                asmc("If previous attempt returns null, make the 'null' string");
                asma("push eax ; save eax");
                asma("push ebx ; push ebx as parameter");
                CALL_FUNCTION("java.lang.String.valueOf$java.lang.Object$");
                asma("pop ebx ; pop old ebx");
                asma("mov ebx, eax ; move created string to ebx");
                asma("pop eax ; pop old eax");

                asml(non_null_lbl);
            }

            if (lhs_type != "java.lang.String" && rhs_type == "java.lang.String") {
                // Push the real rhs string as a parameter
                asma("push eax");
                // Push the real lhs string as (this)
                asma("push ebx");
            } else {
                // Push the real rhs string as a parameter
                asma("push ebx");
                // Push lhs string as (this)
                asma("push eax");
            }
            // Call concat
            CALL_FUNCTION("java.lang.Integer.concat$java.lang.String$");
            asma("pop ebx ; pop this");
            asma("pop ebx ; pop argument");
        } else {
            asmc("CONCAT two strings");
            // Push rhs string as a parameter
            asma("push ebx");
            // Push lhs string as (this)
            asma("push eax");
            // Call concat
            CALL_FUNCTION("java.lang.Integer.concat$java.lang.String$");
            asma("pop ebx ; pop this");
            asma("pop ebx ; pop argument");
        }
    } else if(binExpr->isMinus()) {
        // Specific: JLS 15.18
        asmc("SUBTRACT expr");
        asma("sub eax, ebx");
    } else if(binExpr->isMultiplication()) {
        // Specific: JLS 15.17
        asmc("MULTIPLY expr");
        asma("imul eax, ebx");
    } else if(binExpr->isDivision()) {
        // Specific: JLS 15.17
        std::string lbl_valid_div = LABEL_GEN();

        asmc("DIVIDE expr");
        asma("cmp ebx, 0");
        asma("jne " << lbl_valid_div);
        exceptionCall();
        asml(lbl_valid_div);
        asma("cdq");
        asma("idiv ebx");
    } else if (binExpr->isModulo()) {
        // Specific: JLS 15.17
        std::string lbl_valid_div = LABEL_GEN();

        asmc("MODULO expr");
        asma("cmp ebx, 0");
        asma("jne " << lbl_valid_div);
        exceptionCall();
        asml(lbl_valid_div);
        asma("cdq");
        asma("idiv ebx");
        asma("mov eax, edx");
    } else {
        assert(false);
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
    asmc("ARRAY ACCESS");
    if(access->isArrayAccessName()) {
        traverseAndGenerate(((ArrayAccessName*) access)->getNameOfAccessedArray());
    } else {
        traverseAndGenerate(((ArrayAccessPrimary*) access)->getAccessedPrimaryArray());
    }

    asma("push eax ; push array reference");
    traverseAndGenerate(access->getAccessExpression());

    std::string exceptional_index_access = LABEL_GEN();
    
    asma("mov ebx, eax; save access index");
    asma("pop eax ; get back pushed array reference");
    
    asma("cmp eax, 0 ; check if value of array is null or not");
    asma("je " << exceptional_index_access << " ; check if array is not null");
    
    asma("cmp ebx, 0 ; see if index is less than 0 or not");
    asma("jl " << exceptional_index_access);

    asma("cmp ebx, [eax + 4] ; see if specified index is greater than or equal to length of the array");
    asma("jge " << exceptional_index_access);
    
    std::string proper_index_access = LABEL_GEN();
    asma("jmp " << proper_index_access << " ; everything is fine");
    
    asml(exceptional_index_access);
    exceptionCall();
    
    asml(proper_index_access);
    asma("neg ebx ; negate index");
    asma("mov eax, [eax - " << ObjectLayout::transformToFieldIndexInAnObject(0) << " + 4*ebx]"
         << " ; get the value of the array at the specified index multiplied by 4");
}

void CodeGenerator::traverseAndGenerate(Name* name, CompilationTable** prevTypeForName) {
    // Order implicit based on JLS 15.7
    asmc("ACCESS OF LOCAL VAR/PARAMETER/FIELD FROM NAME");
    
    if (!name->isLastPrefix()) {
        CompilationTable* prevType = NULL;
        traverseAndGenerate(name->getNextName(), &prevType);

        Name* qualifier = name->getNextName();

        if (name->isReferringToField()) {
            assert(prevType != NULL);

            FieldTable* field = name->getReferredField();
            if(qualifier->isReferringToType()) {
                // precautionary check
                assert(field->getField()->isStatic());
                std::string staticName = field->generateFieldLabel();

                asmc("Accessing static variable, get global variable");
                if(prevType != processing) {
                    asma("extern " << staticName);
                }
                asma("mov eax, " << staticName);
            } else {
                std::string null_chk_lbl = LABEL_GEN();

                asma("cmp eax, 0");
                asma("jne " << null_chk_lbl << " ; check if accessed object is null or not");
                exceptionCall();
                asml(null_chk_lbl);
                asma("mov eax, [eax - " << objManager->getLayoutForClass(prevType)->indexOfFieldInObject(field) << "] ; grab field");
            }

            Type* fieldType = name->getReferredField()->getField()->getFieldType();
            if (fieldType->isReferenceType() && prevTypeForName != NULL) {
                *prevTypeForName = ((ReferenceType*) fieldType)->getReferenceTypeTable();
            } //  ELSE IS PRIMITIVE TYPE, BETTER NOT BE A PREFIX
        } else if(name->isReferringToType()) {
            *prevTypeForName = name->getReferredType();
        } else if(name->linkToArrayLength()) {
            std::string null_chk_lbl = LABEL_GEN();

            asma("cmp eax, 0");
            asma("jne " << null_chk_lbl << " ; check if accessed array is null or not");
            exceptionCall();
            asml(null_chk_lbl);
            asma("mov eax, [eax + 4] ; grab array length");
        } // ELSE IS PACKAGE/FIELD DECL/PARAM <- the last two is impossible. We hope.
    } else {
        if (name->isReferringToField()) {
            FieldTable* field = name->getReferredField();
            asma("mov eax, [ebp + 8] ; get this");
            asma("mov eax, [eax - " << objManager->getLayoutForClass(processing)->indexOfFieldInObject(field)
                 << "] ; grab field from this");

            Type* fieldType = field->getField()->getFieldType();
            if (fieldType->isReferenceType() && prevTypeForName != NULL) {
                *prevTypeForName = ((ReferenceType*) fieldType)->getReferenceTypeTable();
            } //  ELSE IS PRIMITIVE TYPE
        } else if(name->isReferringToParameter() || name->isReferringToLocalVar()) {
            SymbolTable* paramOrLocal = getParamOrLocalTableForName(name);
            if (name->isReferringToParameter()) {
                Type* paramType = name->getReferredParameter()->getParameter()->getParameterType();

                if (paramType->isReferenceType() && prevTypeForName != NULL) {
                    *prevTypeForName = static_cast<ReferenceType*>(paramType)->getReferenceTypeTable();
                } //  ELSE IS PRIMITIVE TYPE
            } else {
                Type* localType = name->getReferredLocalVar()->getLocalDecl()->getLocalType();

                if (localType->isReferenceType() && prevTypeForName != NULL) {
                    *prevTypeForName = static_cast<ReferenceType*>(localType)->getReferenceTypeTable();
                } //  ELSE IS PRIMITIVE TYPE
            }

            asma("mov eax, [ebp + " << addressTable[paramOrLocal] << "] ; grab parameter or local variable");
        } else if (name->isReferringToType() && prevTypeForName != NULL) {
            *prevTypeForName = name->getReferredType();
        } // BETTER BE CORRECT - ELSE IS PACKAGE
    }
}

void CodeGenerator::traverseAndGenerate(FieldAccess* access) {
    // Order based on JLS 15.11.1
    asmc("FIELD ACCESS FROM PRIMARY");
    traverseAndGenerate(access->getAccessedFieldPrimary());
    
    // we assume from here on out that the accessed field cannot be
    // a static field
    std::string null_lbl_chk = LABEL_GEN();
    asma("cmp eax, 0 ; check if primary part of field access is 0");
    asma("jne " << null_lbl_chk);
    exceptionCall();
    asml(null_lbl_chk);
    
    if (access->linkToArrayLength()) {
        // accessing the length of an array
        asma("mov eax, [eax + 4] ; get length of array");
    } else if (access->isReferringToField()) {
        // must be accessing some field then
        FieldTable* field = access->getReferredField();
        unsigned int indexInClass = objManager->getLayoutForClass(field->getDeclaringClass())->indexOfFieldInObject(field);
        asma("mov eax, [eax - " << indexInClass << "] ; access field from some class");
    } else {
        assert(false);
    }
}

void CodeGenerator::traverseAndGenerate(MethodInvoke* invoke) {
    // Order based on JLS 15.12.4
    asmc("METHOD INVOCATION");
    bool targetReferencePushed = false;

    if(invoke->isNormalMethodCall()) {
        Name* prefixOfMethod = ((MethodNormalInvoke*) invoke)->getNameOfInvokedMethod()->getNextName();
        if(prefixOfMethod != NULL && !prefixOfMethod->isReferringToType()) {
            // precautionary check
            assert(!(invoke->isReferringToClassMethod() &&
                     invoke->getReferredClassMethod()->getClassMethod()->isStatic()));
            // there's a prefix to the method call and
            // the prefix does not refer to a type
            // i.e this is not a static call
            
            // get the value of the prefix first
            traverseAndGenerate(prefixOfMethod);
            targetReferencePushed = true;
        }
    } else {
        // method call through a field access
        // assumption is that the method called is then
        // not static
        
        // get the value of the prefix first
        traverseAndGenerate(((InvokeAccessedMethod*) invoke)->getAccessedMethod()->getAccessedFieldPrimary());
        targetReferencePushed = true;
    }

    if(targetReferencePushed) {
        // if temporary storage was used, then
        // check that the prefix's value is not null
        asma("cmp eax, 0 ; check that prefix's value is not null");
        std::string null_chk_lbl = LABEL_GEN();
        asma("jne " << null_chk_lbl);
        exceptionCall();
        asml(null_chk_lbl);
    }

    bool isNativeMethod = invoke->isReferringToClassMethod() && invoke->getReferredClassMethod()->getClassMethod()->isNative();
    if(!isNativeMethod) {
        // not a native method
        traverseAndGenerate(invoke->getArgsForInvokedMethod(), targetReferencePushed);
    } else {
        // a native method, guaranteed then to have only
        // one argument of type int
        traverseAndGenerate(invoke->getArgsForInvokedMethod()->getListOfArguments()->getSelfArgumentExpr());
    }

    if (invoke->isReferringToClassMethod() &&
            invoke->getReferredClassMethod()->getClassMethod()->isStatic()) {
        // a call to a static class method
        ClassMethodTable* classMethod = invoke->getReferredClassMethod();
        if(!isNativeMethod) {
            asmc("STATIC METHOD INVOCATION");
            // not a native method being called
            CALL_FUNCTION(classMethod->generateMethodLabel());
        } else {
            asmc("NATIVE METHOD INVOCATION");
            std::string method = invoke->getReferredClassMethod()->getDeclaringClass()->getCanonicalName() + "." +
                                 ((MethodNormalInvoke*) invoke)->getNameOfInvokedMethod()->getNameId()->getIdAsString();
            CALL_FUNCTION(LabelManager::labelizeToNativeMethod(method));
            // done nothing else to do
            return;
        }

    } else if (invoke->isReferringToInterfaceMethod() || invoke->isReferringToClassMethod()) {
        // if here, then must be calling a non-static method
        if (invoke->isReferringToInterfaceMethod()) {
            asmc("METHOD INVOCATION THROUGH INTERFACE TABLE");
        } else {
            asmc("METHOD INVOCATION THROUGH VIRTUAL TABLE");
        }

        if(targetReferencePushed) {
            // a target reference was computed
            // push eax, which should have the target reference
            asma("push eax ; push target reference");
        } else {
            // no target reference was computed
            // then use this instead
            asma("mov eax, [ebp + 8] ; get this");
            asma("push eax ; push this as target reference");
        }

        if(invoke->isReferringToInterfaceMethod()) {
            asma("mov eax, [eax - 8] ; get interface table");
        } else {
            asma("mov eax, [eax] ; get virtual table");
        }

        if (invoke->isReferringToInterfaceMethod()) {
            InterfaceMethodTable* interfaceMethod = invoke->getReferredInterfaceMethod();

            std::string interfaceMethodSignature = interfaceMethod->getInterfaceMethod()->methodSignatureAsString();
            asma("call [eax + " << interManager->getInterfaceMethodIndexInTable(interfaceMethodSignature) << "]");

        } else {
            ClassMethodTable* classMethod = invoke->getReferredClassMethod();
            
            std::string typeCanonicalName = classMethod->getDeclaringClass()->getCanonicalName();
            std::string signature = classMethod->getClassMethod()->getMethodHeader()->methodSignatureAsString();
            asma("call [eax + " <<
                 virtualManager->getVTableLayoutForType(typeCanonicalName)->getIndexOfMethodInVTable(signature) << "]");
        }
    } else {
        assert(false);
    }

    if(targetReferencePushed) {
        // some target reference was pushed
        asma("pop ebx ; pop target reference from stack");
    }

    Arguments* args = invoke->getArgsForInvokedMethod()->getListOfArguments();
    while(args != NULL) {
        asma("pop ebx ; popping arguments");
        args = args->getNextArgs();
    }
}

void CodeGenerator::traverseAndGenerate(ArgumentsStar* args, bool targetReferencePushed) {
    if(!args->isEpsilon()) {
        asmc("ARRANGE ARGUMENTS LEFT-MOST AT THE BOTTOM, RIGHT-MOST AT THE TOP");
        traverseAndGenerate(args->getListOfArguments(), targetReferencePushed);
    }
}

void CodeGenerator::traverseAndGenerate(Arguments* arg, bool targetReferencePushed) {
    if(!arg->lastArgument()) {
        // traverse what ever is to the left of me first
        traverseAndGenerate(arg->getNextArgs(), targetReferencePushed);
    }

    if(targetReferencePushed) {
        asma("push eax ; push target reference");
    }

    traverseAndGenerate(arg->getSelfArgumentExpr());
    if(targetReferencePushed) {
        asma("pop ebx ; get back target reference");
    }

    asma("push eax ; now push self");

    if(targetReferencePushed) {
        asma("mov eax, ebx ; move over target reference back to eax");
    }
}

void CodeGenerator::traverseAndGenerate(NewClassCreation* create) {
    // Order based on JLS 15.9.4, with a twise that arguments are evaluated first
    // then space for class is created
    asmc("NEW CLASS CREATION");
    traverseAndGenerate(create->getArgsToCreateClass());
    
    CALL_FUNCTION(create->getReferredConstructor()->getConstructor()->labelizedConstructorSignature());
    asma("pop eax ; pop newly created object to eax");
    
    Arguments* args = create->getArgsToCreateClass()->getListOfArguments();
    while(args != NULL) {
        asma("pop ebx ; pop arguments to constructor");
        args = args->getNextArgs();
    }
}

void CodeGenerator::traverseAndGenerate(PrimaryNewArray* newArray) {
    // Order based on JLS 15.10.1
    asmc("NEW ARRAY CREATION");
    traverseAndGenerate(newArray->getTheDimension());

    std::string less_than_zero_chk = LABEL_GEN();
    asma("cmp eax, 0");
    asma("jge " << less_than_zero_chk << " ; check if dimension expression is less than zero or not");
    exceptionCall();
    asml(less_than_zero_chk);
    arrayCreationCall("eax * 4");
    
    Type* arrayType = newArray->getArrayType();
    std::string labelizedInhTable = LabelManager::getLabelForArrayInheritanceTable(arrayType->getTypeAsString());

    if(arrayType->isReferenceType()) {
        asma("mov [eax + 8], " << inhManager->getTypeMapping(arrayType->getTypeAsString()) << " ; insert component type");
    }
    asma("extern " << labelizedInhTable);
    asma("mov [eax - 4], " << labelizedInhTable << " ; insert array inheritance table");
    asma("mov [eax - 12], " << inhManager->getTypeMapping(arrayType->getTypeAsString() + "[]") << " ; insert type number");
}

void CodeGenerator::traverseAndGenerate(QualifiedThis* qual) {
    // JLS 15.8.4
    asmc("QUALIFIED THIS");
    asma("mov eax, [ebp + 8] ; put 'this' into eax");
}

void CodeGenerator::traverseAndGenerate(LiteralOrThis* lit) {
    if(lit->isThis()) {
        // JLS 15.8.3
        asmc("This literal");
        asma("mov eax, [ebp + 8] ; put 'this' into eax");
    } else if(lit->isNumber()) {
        // JLS 15.8.1
        asmc("Number Literal");
        asma("mov eax, " << lit->getLiteralToken()->getString());
    } else if(lit->isTrueBoolean()) {
        // JLS 15.8.1
        asmc("True Boolean literal");
        asma("mov eax, 1");
    } else if(lit->isFalseBoolean()) {
        // JLS 15.8.1
        asmc("False Boolean literal");
        asma("mov eax, 0");
    } else if(lit->isCharLiteral()) {
        // JLS 15.8.1
        std::string character = lit->getLiteralToken()->getString();

        asmc("Character literal");
        asma("mov eax, " << character);
    } else if(lit->isStringLiteral()) {
        // JLS 15.8.1
        std::string string_literal = lit->getLiteralToken()->getString();
        string_literal.erase(std::remove(string_literal.begin(), string_literal.end(), '"'), string_literal.end());

        asmc("String literal");
        // Create character array to hold string
        std::stringstream ss;
        ss << (string_literal.length() * 4);
        arrayCreationCall(ss.str());

        std::string charArrayInheritance = LabelManager::getLabelForArrayInheritanceTable("char");
        asmc("Insert inheritance table of char[]");
        asma("extern " << charArrayInheritance);
        asma("mov [eax - 4], " << charArrayInheritance);
        asmc("Insert char[]'s type number");
        asma("mov [eax - 12], " << inhManager->getTypeMapping("char[]"));
        // Copy over string into array
        unsigned int offset = ObjectLayout::transformToFieldIndexInAnObject(0);
        for (unsigned int i = 0; i < string_literal.length(); i++) {
            asma("mov [eax - " << offset << "], " << ((int)string_literal[i]));
            offset += 4;
        }

        asmc("Call String constructor");
        asma("push eax");
        CALL_FUNCTION(LabelManager::labelizeForConstructor("java.lang.String$" + LabelManager::labelizeForArrays("char") + "$"));
        //CALL_FUNCTION(LabelManager::labelizeForConstructor("java.lang.String", 1, "char.array"));
        asma("pop eax ; pop created string into eax");
        asma("pop ebx ; pop old eax");
    } else if (lit->isNull()) {
        // JLS 15.8.1
        asmc("Null literal");
        asma("mov eax, 0");
    } else {
        assert(false);
    }
}

void CodeGenerator::traverseAndGenerate(NegationExpression* negExpr) {
    // Order based on JLS 15.7
    traverseAndGenerate(negExpr->getNegatedExpression());
    if(negExpr->isNumericNegation()) {
        // Specific: JLS 15.18.2
        asmc("Numeric Negation");
        asma("neg eax");
    } else {
        // Specific: JLS 15.15.6
        // boolean negation
        asmc("Boolean Negation");
        asma("xor eax, 1");
    }
}

void CodeGenerator::traverseAndGenerate(CastExpression* cast) {
    // Order based on JLS 15.7
    // Specific: JLS 15.16 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(cast->getExpressionToCast());

    if ((cast->isCastToPrimitiveType() && (((CastPrimitive*) cast)->isPrimitiveArrayCast())) ||
        cast->isCastToReferenceType() || cast->isCastToArrayName()) {
        if (cast->isCastToPrimitiveType()) {
            asmc("Casting to primitive array");
        } else if (cast->isCastToReferenceType()) {
            asmc("Casting to reference type");
        } else {
            asmc("Casting to reference array");
        }
        
        std::string all_good_lbl = LABEL_GEN();
        asma("cmp eax, 0");
        asma("je " << all_good_lbl << " ; check if expression value is null");
        
        asma("mov ebx, [eax - 4] ; grab inheritance table");
        
        if (cast->isCastToPrimitiveType()) {
            CastPrimitive* castPrim = (CastPrimitive*) cast;
            asma("mov ebx, [ebx + " << inhManager->getTypeMapping(castPrim->getPrimitiveTypeToCastTo()->getTypeAsString())
                  << "] ; grab inheritance value");
        } else {
            CastName* castRef = (CastName*) cast;
            asma("mov ebx, [ebx + " << inhManager->getTypeMapping(castRef->getTypeToCastAsString()) << "] ; grab inheritance value");
        }
        
        asmc("Check if expression can be casted to");
        asma("cmp ebx, 0");
        asma("jne " << all_good_lbl);
        exceptionCall();
        
        asml(all_good_lbl);
   } else {
       // easy case
       Expression* castedExpr = cast->getExpressionToCast();
       Type* primType = ((CastPrimitive*) cast)->getPrimitiveTypeToCastTo();
       if (primType->isTypeShort() && castedExpr->isExprTypeInt()) {
           asmc("Casting type integer to type short");
           asma("shl eax, 16 ; shift left by 16, throwing off last 16 MSB");
           asma("sar eax, 16 ; shift right by 16, retain last 16 LSB and sign");
       } else if ((primType->isTypeByte() || primType->isTypeChar()) &&
               (castedExpr->isExprTypeShort() || castedExpr->isExprTypeInt())) {
           asmc("Casting type short or integer to byte or char");
           asma("shl eax, 24 ; shift left by 24, throwing off last 24 MSB");
           asma("sar eax, 24 ; shift right by 24, retain last 8 LSB and sign");
       } // The rest doesn't need anything to be done, since
         // the bits will remain the same way for all primitive types
    }
}

void CodeGenerator::traverseAndGenerate(InstanceOf* instanceof) {
    // Order based on JLS 15.7
    // Specific: JLS 15.20.2 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(instanceof->getExpressionToCheck());

    std::string null_lbl_chk = LABEL_GEN();
    std::string done_chk = LABEL_GEN();
    asma("cmp eax, 0 ; check if expression evaluates to null");
    asma("jne " << null_lbl_chk);
    asma("jmp " << done_chk << " ; finish instanceof check because expression evaluates to null");

    asml(null_lbl_chk);
    asma("mov eax, [eax - 4] ; grab inheritance table");
    
    Type* checkType = instanceof->getTypeToCheck();
    asma("mov eax, [eax + " << inhManager->getTypeMapping(checkType->getTypeAsString()) << "] ; grab what the inheritance table says");
    asml(done_chk);
}

void CodeGenerator::traverseAndGenerate(Assignment* assign) {
    // Order based on JLS 15.7
    // Specific: JLS 15.26
    if(assign->isAssignName()) {
        asmc("ASSIGNMENT OF NAME");
        traverseAndGenerate(((AssignName*) assign)->getNameToAssign());
    } else if(assign->isAssignField()) {
        asmc("ASSIGNMENT OF ACCESSED FIELD");
        traverseAndGenerate(((AssignField*) assign)->getAssignedField());
    } else {
        asmc("ASSIGNMENT OF ARRAY COMPONENT");
        // assigning to an array component
        ArrayAccess* accessed = ((AssignArray*) assign)->getAssignedArray();
        if(accessed->isArrayAccessName()) {
            // accessing an array from a name
            traverseAndGenerate(((ArrayAccessName*) accessed)->getNameOfAccessedArray());
        } else {
            // accessing an array from a primary
            traverseAndGenerate(((ArrayAccessPrimary*) accessed)->getAccessedPrimaryArray());
        }
    }
    
    asma("push eax ; push gotten lhs reference value");
    if(assign->isAssignArray()) {
        traverseAndGenerate(((AssignArray*) assign)->getAssignedArray()->getAccessExpression());
        asma("push eax ; push access index");
    }

    Expression* assignExpr = assign->getExpressionToAssign();
    traverseAndGenerate(assignExpr);

    asma("mov ebx, eax ; move rhs value to ebx");
    if(assign->isAssignArray()) {
        asma("pop edx ; get back access index");
        asma("pop eax ; get back referenced array");

        ArrayAccess* accessed = ((AssignArray*) assign)->getAssignedArray();
        if(accessed->isArrayAccessName()) {
            traverseAndGenerate(((ArrayAccessName*) accessed)->getNameOfAccessedArray());
        } else {
            traverseAndGenerate(((ArrayAccessPrimary*) accessed)->getAccessedPrimaryArray());
        }
        
        std::string exceptional_access = LABEL_GEN();
        asma("cmp eax, 0");
        asma("je " <<  exceptional_access << " ; check if array accessed is null or not");

        asma("cmp edx, 0");
        asma("jl " << exceptional_access << " ; check if access index is less than 0 or not");
        
        asma("cmp edx, [eax + 4]");
        asma("jge " << exceptional_access << " ; check if access index is greater than or equal to array length");

        std::string proper_array_access = LABEL_GEN();
        asma("jmp " << proper_array_access);

        asml(exceptional_access);
        exceptionCall();
        
        asml(proper_array_access);
        if(accessed->isAccessingObjectArray()) {
            asmc("Accessing an array of reference");
            asma("mov esi, [eax + 8] ; grab component type number");

            std::string good_assignment = LABEL_GEN();
            asma("cmp ebx, 0");
            asma("je " << good_assignment << " ; check if rhs value is null or not");
            
            asma("mov ecx, [ebx - 4] ; grab rhs inheritance table");
            asma("mov esi, [ecx + esi] ; grab inheritance status from inheritance table");
            
            asma("cmp esi, 1");
            asma("je " << good_assignment << " ; check if rhs value is a subclass of component type of array");
            exceptionCall();

            asml(good_assignment);
            asma("neg edx ; negate index");
            asma("mov eax, [eax - " << ObjectLayout::transformToFieldIndexInAnObject(0) << " + 4*edx] ; get array component at index");
        }
    } else {
        asma("pop eax ; get back lhs reference value");
    }
    asma("mov [eax], ebx ; store the rhs value to the memory location referred to in eax");
    asma("mov eax, ebx ; store the rhs value to eax for the return value of an assignment");
}

void CodeGenerator::traverseAndGenerate(ClassMethod* method) {
    if(!method->getMethodBody()->noDefinition()) {
        std::string signature = processing->getCanonicalName() + "." + method->getMethodHeader()->labelizedMethodSignature();
        asmc("Method Body - " << signature);
        asmgl(signature);
        CALL_IDIOM();
        scope_offset = -4;

        // Add Parameters to the address table
        // If static there is no implicit this
        int param_offset = method->isStatic() ? 8 : 12;

        setParameterOffsetFromEBP(method->getMethodHeader()->getClassMethodParams()->getListOfParameters(), param_offset);
        // the method has a body, then generate code
        // for the body
        traverseAndGenerate(method->getMethodBody());

        if(method->getMethodBody()->canMethodCompleteNormally()) {
            // by this stage all methods that can complete normally
            // are methods that have return type void
            asmc("Implicit Return");
            RETURN_IDIOM();
        }
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
    std::string id = local->getLocalId()->getIdAsString();

    // Everything primitive/reference type/array will
    // be stored in a DD (32 bits -> 4 bytes) on the stack
    asmc("Local Decl - " << id);
    addressTable[local->getLocalTable()] = scope_offset;
    scope_offset -= 4;

    // Set the value of the expression to the new declaration
    traverseAndGenerate(local->getLocalInitExpr());
    asmc("Initialize Local Decl - " << id);
    asma("push eax ; store local variable on stack");
}

void CodeGenerator::traverseAndGenerate(IfStmt* stmt) {
    // Order based on JLS 14.9
    asmc("If Statement");
    traverseAndGenerate(stmt->getExpressionToEvaluate());

    std::string lbl_false = LABEL_GEN();
    std::string lbl_end = LABEL_GEN();
    // Check if expression is true, if not jump to lbl_false
    asma("cmp eax, 1");
    asma("jne " << lbl_false);
    traverseAndGenerate(stmt->getExecuteTruePart());
    asma("jmp " << lbl_end);

    // ELSE
    asmc("If Statement Else");
    asml(lbl_false);
    if(!stmt->noElsePart()) {
        traverseAndGenerate(stmt->getExecuteFalsePart());
    }

    // END
    asmc("If statement end");
    asml(lbl_end);
}

void CodeGenerator::traverseAndGenerate(WhileStmt* stmt) {
    // Order based on JLS 14.11
    std::string lbl_begin = LABEL_GEN();
    std::string lbl_end = LABEL_GEN();

    asmc("While statement");

    // Check expression is true, if not lbl_end
    asml(lbl_begin);
    traverseAndGenerate(stmt->getExpressionToEvaluate());
    asma("cmp eax, 1");
    asma("jne " << lbl_end);

    asmc("While statement body");
    // If true run loop statement
    traverseAndGenerate(stmt->getLoopStmt());
    asma("jmp " << lbl_begin);

    // END
    asmc("While statement end");
    asml(lbl_end);
}

void CodeGenerator::traverseAndGenerate(ForStmt* stmt) {
    // Order based on JLS 14.13.2
    asmc("For statement init");
    int saved_scope_offset = scope_offset;
    if(!stmt->emptyForInit()) {
        traverseAndGenerate(stmt->getForInit());
    }

    std::string lbl_begin = LABEL_GEN();
    std::string lbl_end = LABEL_GEN();

    // Check expression is true, if not lbl_end
    asml(lbl_begin);
    if(!stmt->emptyExpression()) {
        asmc("For Statement condition");
        traverseAndGenerate(stmt->getExpressionToEvaluate());
        asma("cmp eax, 1");
        asma("jne " << lbl_end);
    }

    asmc("For statement body");
    // If true run loop statement and update
    traverseAndGenerate(stmt->getLoopStmt());
    if(!stmt->emptyForUpdate()) {
        asmc("For statement update");
        traverseAndGenerate(stmt->getForUpdate());
    }
    asma("jmp " << lbl_begin);

    // END
    asmc("For statement end");
    asml(lbl_end);
    scope_offset = saved_scope_offset;
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
        int saved_scope_offset = scope_offset;
        traverseAndGenerate(stmt->getNestedBlock());
        scope_offset = saved_scope_offset;
    }
}

void CodeGenerator::traverseAndGenerate(ReturnStmt* stmt) {
    // JLS 14.16
    traverseAndGenerate(stmt->getReturnExpr());
    RETURN_IDIOM();
}

void CodeGenerator::traverseAndGenerate(Constructor* ctor) {
    // JLS 12.5 for what happens when a constructor is called
    std::string signature = ctor->labelizedConstructorSignature();

    asmc("Constructor Body - " << signature);
    asmgl(signature);
    CALL_IDIOM();

    CompilationTable* superclass = ((ClassTable*) processing->getSymbolTable())->getClass()->getSuper()->getSuperClassTable();
    if(superclass != NULL) {
        // there is a superclass -> then call the superclass
        // zero argument constructor
        asma("push dword [ebp - 8] ; push created this onto the stack before calling super constructor");
        std::string superctor = superclass->getAConstructor("()")->getConstructor()->labelizedConstructorSignature();
        asma("extern " << superctor);
        asma("call " << superctor);
        asma("pop ebx ; pop pushed this");
    }

    // call initializers here
    callInitializersOfDeclaredFields();

    // set offset for constructor parameters from EBP,
    // starting offset is always 12 for constructors, because
    // there's always this
    setParameterOffsetFromEBP(ctor->getConstructorParameters()->getListOfParameters(), 12);

    scope_offset = -4;
    traverseAndGenerate(ctor->getConstructorBody());

    if(ctor->canConstructorCompleteNormally()) {
        asmc("Implicit Return");
        RETURN_IDIOM();
    }
}
