#include <cassert>
#include <iostream>

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

// Symbol table
#include "compilationTable.h"
#include "classTable.h"

// Code generation related
#include "codeGenerator.h"
#include "startup.h"
// Managers
#include "vtableManager.h"
#include "objectLayoutManager.h"
#include "inheritanceTableManager.h"
#include "implementedInterfaceMethodTableManager.h"
#include "staticFieldsManager.h"

// Label
#include "labelManager.h"
#include "RLG.h"

void CodeGenerator::CALL_FUNCTION(std::string fn_name) {
    asma("push ebp");
    asma("mov ebp, esp");
    asma("extern " << fn_name);
    asma("call " << fn_name);
    asma("mov esp, ebp");
    asma("pop ebp");
}

CodeGenerator::CodeGenerator(std::map<std::string, CompilationTable*>& compilations, CompilationTable* firstUnit) :
            compilations(compilations), starter(new Startup(compilations, firstUnit)),
            virtualManager(new VTableManager(compilations)), objManager(new ObjectLayoutManager(compilations)),
            inhManager(new InheritanceTableManager(compilations)), interManager(new ImplInterfaceMethodTableManager(compilations)),
            staticManager(new StaticFieldsManager(compilations)), fs(NULL) {}

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
            fs = new std::ofstream(it->second->getCanonicalName() + ".s");
            traverseAndGenerate(((ClassTable*)it->second->getSymbolTable())->getClass());
            delete fs;
            // fs = NULL;
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
        traverseAndGenerate(field->getInitializingExpression());
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

    // Order based on JLS 15.7.2, except || (azy or) and && (lazy and)
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
        } else if ((lhs_type == "java.lang.String" && rhs_type != "java.lang.String") &&
                   (lhs_type != "java.lang.String" && rhs_type == "java.lang.String")) {
            // Fake it and swap lhs with rhs
            if (lhs_type != "java.lang.String" && rhs_type == "java.lang.String") {
                Expression* temp = lhs_expr;
                lhs_expr = rhs_expr;
                rhs_expr = temp;

                asma("mov edx, eax");
                asma("mov eax, ebx");
                asma("mov ebx, edx");
            }

            // Primitive Type
            // - Create object for primitive, then call toString
            if (rhs_expr->isExprTypeInt()) {
                asmc("CONCAT string with int primitive");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for constructor parameter
                asma("push ebx");
                // Call Constructor for java.lang.Integer
                CALL_FUNCTION("java.lang.Integer$int$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for toString (this)
                asma("push ebx");
                // Call toString on java.lang.Integer
                CALL_FUNCTION("java.lang.Integer.toString$$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            } else if (rhs_expr->isExprTypeShort()) {
                asmc("CONCAT string with short primitive");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for constructor parameter
                asma("push ebx");
                // Call Constructor for java.lang.Short
                CALL_FUNCTION("java.lang.Short$short$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push short for toString (this)
                asma("push ebx");
                // Call toString on java.lang.Short
                CALL_FUNCTION("java.lang.Short.toString$$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            } else if (rhs_expr->isExprTypeByte()) {
                asmc("CONCAT string with byte primitive");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for constructor parameter
                asma("push ebx");
                // Call Constructor for java.lang.Byte
                CALL_FUNCTION("java.lang.Byte$byte$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push short for toString (this)
                asma("push ebx");
                // Call toString on java.lang.Byte
                CALL_FUNCTION("java.lang.Byte.toString$$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            } else if (rhs_expr->isExprTypeChar()) {
                asmc("CONCAT string with character primitive");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for constructor parameter
                asma("push ebx");
                // Call Constructor for java.lang.Character
                CALL_FUNCTION("java.lang.Character$char$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push short for toString (this)
                asma("push ebx");
                // Call toString on java.lang.Character
                CALL_FUNCTION("java.lang.Character.toString$$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            } else if (rhs_expr->isExprTypeBoolean()) {
                asmc("CONCAT string with boolean primitive");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push integer for constructor parameter
                asma("push ebx");
                // Call Constructor for java.lang.Boolean
                CALL_FUNCTION("java.lang.Boolean$boolean$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push short for toString (this)
                asma("push ebx");
                // Call toString on java.lang.Boolean
                CALL_FUNCTION("java.lang.Boolean.toString$$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            } else if (rhs_expr->getExpressionTypeString() == "null") {
                asmc("CONCAT string with null");
                // Save eax to prevent thrashing
                asma("push eax");
                // Create an array of size 4
                asma("mov eax, 4");
                CALL_FUNCTION("makeArrayBanana");
                // Write the word "null" in char array
                asma("mov [eax + 16], 110 ;; n");
                asma("mov [eax + 20], 117 ;; u");
                asma("mov [eax + 24], 108 ;; l");
                asma("mov [eax + 28], 108 ;; l");
                asma("mov eax, ebx");
                asma("pop eax");

                // Save eax to prevent thrashing
                asma("push eax");
                // Push char[] for constructor of java.lang.String
                asma("push ebx");
                // Call constructor for java.lang.String(char.array)
                CALL_FUNCTION("java.lang.String$char.array$");
                asma("pop ebx");
                asma("mov eax, ebx");
                asma("pop eax");
            } else {
                // Reference Type
                asmc("CONCAT string with reference type");
                // Save eax to prevent thrashing
                asma("push eax");
                // Push object for valueOf parameter
                asma("push ebx");
                // Call valueOf function in java.lang.String
                CALL_FUNCTION("java.lang.String.valueOf$java.lang.Object$");
                asma("pop ebx");
                asma("mov ebx, eax");
                asma("pop eax");
            }

            if (lhs_type != "java.lang.String" && rhs_type == "java.lang.String") {
                // Push the real rhs string as a parameter
                asma("push eax");
                // Push the real lhs string as (this)
                asma("push ebx");
            } else {
                // Push new integer string as a parameter
                asma("push ebx");
                // Push lhs string as (this)
                asma("push eax");
            }
            // Call concat
            CALL_FUNCTION("java.lang.Integer.concat$java.lang.String$");
            asma("pop ebx");
            asma("pop ebx");
        } else {
            asmc("CONCAT two strings");
            // Push new integer string as a parameter
            asma("push ebx");
            // Push lhs string as (this)
            asma("push eax");
            // Call concat
            CALL_FUNCTION("java.lang.Integer.concat$java.lang.String$");
            asma("pop ebx");
            asma("pop ebx");
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
        asma("extern __exception");
        asma("call __exception");
        asml(lbl_valid_div);
        asma("cdq");
        asma("idiv ebx");
    } else if (binExpr->isModulo()) {
        // Specific: JLS 15.17
        // TODO: Check that yea.
        std::string lbl_valid_div = LABEL_GEN();

        asmc("MODULO expr");
        asma("cmp ebx, 0");
        asma("jne " << lbl_valid_div);
        asma("extern __exception");
        asma("call __exception");
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
    if(access->isArrayAccessName()) {
        traverseAndGenerate(((ArrayAccessName*) access)->getNameOfAccessedArray());
    } else {
        traverseAndGenerate(((ArrayAccessPrimary*) access)->getAccessedPrimaryArray());
    }
    traverseAndGenerate(access->getAccessExpression());
}

void CodeGenerator::traverseAndGenerate(Name* name) {
    // Order implicit based on JLS 15.7
    if(!name->isLastPrefix()) {
        traverseAndGenerate(name->getNextName());
    }
}

void CodeGenerator::traverseAndGenerate(FieldAccess* access) {
    // Order based on JLS 15.11.1
    traverseAndGenerate(access->getAccessedFieldPrimary());
}

void CodeGenerator::traverseAndGenerate(MethodInvoke* invoke) {
    // Order based on JLS 15.12.4
    traverseAndGenerate(invoke->getArgsForInvokedMethod());
    if(invoke->isNormalMethodCall()) {
    } else {
    }
}

void CodeGenerator::traverseAndGenerate(ArgumentsStar* args) {
    if(!args->isEpsilon()) {
        traverseAndGenerate(args->getListOfArguments());
    }
}

void CodeGenerator::traverseAndGenerate(Arguments* arg) {
    // Left to right on the stack, with this at the top
    if(!arg->lastArgument()) {
        traverseAndGenerate(arg->getNextArgs());
    }
}

void CodeGenerator::traverseAndGenerate(NewClassCreation* create) {
    // Order based on JLS 15.9.4
    traverseAndGenerate(create->getArgsToCreateClass());
}

void CodeGenerator::traverseAndGenerate(PrimaryNewArray* newArray) {
    // Order based on JLS 15.10.1
    traverseAndGenerate(newArray->getTheDimension());
}

void CodeGenerator::traverseAndGenerate(QualifiedThis* qual) {
    // JLS 15.8.4
}

void CodeGenerator::traverseAndGenerate(LiteralOrThis* lit) {
    if(lit->isThis()) {
        // JLS 15.8.3
        asmc("This literal");
        asma("mov eax, [ebp + 8]");
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
        character.erase(std::remove(character.begin(), character.end(), '\''), character.end());

        asmc("Character literal");
        asma("mov eax, " << ((int)(character.c_str()[0])));
    } else if(lit->isStringLiteral()) {
        // JLS 15.8.1
        std::string string_literal = lit->getLiteralToken()->getString();

        asmc("String literal");
        // Create character array to hold string
        asma("mov eax, " << string_literal.length());
        CALL_FUNCTION("makeArrayBanana");
        // Copy over string into array
        unsigned int offset = 16;
        for (unsigned int i = 0; i < string_literal.length(); i++) {
            asma("mov [eax + " << offset << "], " << ((int)string_literal[i]));
            offset += 4;
        }
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
    } else {
        // Specific: JLS 15.15.6
        // boolean negation
        asmc("Boolean Negation");
        asma("xor eax, 1");
    }
}

void CodeGenerator::traverseAndGenerate(CastExpression* cast) {
    // Order based on JLS 15.7
    // Specific: JLS 15.20.2 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(cast->getExpressionToCast());
    if(cast->isCastToPrimitiveType()) {
    } else {
        // cast to array or some reference type
    }
}

void CodeGenerator::traverseAndGenerate(InstanceOf* instanceof) {
    // Order based on JLS 15.7
    // Specific: JLS 15.20.2 and JLS 5.2 (assignment conversion rules for Joos, review A3 type-checking)
    traverseAndGenerate(instanceof->getExpressionToCheck());
}

void CodeGenerator::traverseAndGenerate(Assignment* assign) {
    // Order based on JLS 15.7
    // Specific: JLS 15.26
    if(assign->isAssignName()) {
        traverseAndGenerate(((AssignName*) assign)->getNameToAssign());
    } else if(assign->isAssignField()) {
        traverseAndGenerate(((AssignField*) assign)->getAssignedField());
    } else {
        // assigning to an array component
        traverseAndGenerate(((AssignArray*) assign)->getAssignedArray());
    }
    
    traverseAndGenerate(assign->getExpressionToAssign());
}

void CodeGenerator::traverseAndGenerate(ClassMethod* method) {
    if(!method->getMethodBody()->noDefinition()) {
        // the method has a body, then generate code
        // for the body
        traverseAndGenerate(method->getMethodBody());
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
    traverseAndGenerate(local->getLocalInitExpr());
}

void CodeGenerator::traverseAndGenerate(IfStmt* stmt) {
    // Order based on JLS 14.9
    traverseAndGenerate(stmt->getExpressionToEvaluate());
    traverseAndGenerate(stmt->getExecuteTruePart());
    if(!stmt->noElsePart()) {
        traverseAndGenerate(stmt->getExecuteFalsePart());
    }
}

void CodeGenerator::traverseAndGenerate(WhileStmt* stmt) {
    // Order based on JLS 14.11
    traverseAndGenerate(stmt->getExpressionToEvaluate());
    traverseAndGenerate(stmt->getLoopStmt());
}

void CodeGenerator::traverseAndGenerate(ForStmt* stmt) {
    // Order based on JLS 14.13.2
    if(!stmt->emptyForInit()) {
        traverseAndGenerate(stmt->getForInit());
    }

    traverseAndGenerate(stmt->getForUpdate());
    traverseAndGenerate(stmt->getLoopStmt());

    if(!stmt->emptyForUpdate()) {
        traverseAndGenerate(stmt->getForUpdate());
    }
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
        traverseAndGenerate(stmt->getNestedBlock());
    }
}

void CodeGenerator::traverseAndGenerate(ReturnStmt* stmt) {
    // JLS 14.16
    traverseAndGenerate(stmt->getReturnExpr());
}

void CodeGenerator::traverseAndGenerate(Constructor* ctor) {
    traverseAndGenerate(ctor->getConstructorBody());
}
