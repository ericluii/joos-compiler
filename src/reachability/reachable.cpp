#include <sstream>
#include <cassert>

#include "error.h"
#include "reachable.h"

#include "compilationTable.h"
#include "classTable.h"
#include "classBodyStar.h"
#include "classBodyDecls.h"
#include "classMethod.h"
#include "methodHeader.h"
#include "methodBody.h"
#include "blockStmtsStar.h"
#include "blockStmts.h"
#include "nestedBlock.h"
#include "ifStmt.h"
#include "whileStmt.h"
#include "binaryExpression.h"
#include "primaryExpression.h"
#include "literalOrThis.h"

#include "constructor.h"

Reachable::Reachable(std::map<std::string, std::vector<CompilationTable*> >& compilations) : compilations(compilations),
        curCompilation(NULL), prevStmtReachable(true), prevStmtComplete(true), curReturnType(NULL),
        unreachableStmt(false), inConstructor(false) {}

// -----------------------------------------------------------
// Private use

void Reachable::checkReachability(CompilationTable* table) {
    curCompilation = table;
    if(table->aTypeWasDefined() && table->isClassSymbolTable()) {
        // only if a type was defined and the type is a class
        checkReachability(((ClassTable*) table->getSymbolTable())->getClass());
    }
}

void Reachable::checkReachability(ClassDecl* aClass) {
    checkReachability(aClass->getClassMembers());
}

void Reachable::checkReachability(ClassBodyStar* body) {
    if(!body->isEpsilon()) {
        checkReachability(body->getBody());
    }
}

void Reachable::checkReachability(ClassBodyDecls* body) {
    if(!body->isLastClassMember()) {
        checkReachability(body->getNextDeclaration());
    }

    if(!unreachableStmt) {
        // if no unreachability error has occurred
        if(body->isClassMethod()) {
            checkReachability((ClassMethod*) body);
        } else if(body->isConstructor()) {
            checkReachability((Constructor*) body);
        }
    }
}

void Reachable::checkReachability(ClassMethod* method) {
    // set the return type
    curReturnType = method->getMethodHeader()->getReturnType();
    checkReachability(method->getMethodBody());
}

void Reachable::checkReachability(MethodBody* body) {
    if(!body->noDefinition()) {
        checkReachability(body->getBlockStmtsStar());
    }
}

void Reachable::checkReachability(BlockStmtsStar* stmts) {
    if(!stmts->isEpsilon()) {
        checkReachability(stmts->getStatements());
    }
}

void Reachable::checkReachability(BlockStmts* stmt) {
    if(!stmt->isLastStatement()) {
        checkReachability(stmt->getNextBlockStmt());
    }

    if(unreachableStmt) {
        // a previous stmt is unreachable and has been checked, then silently return
        return;
    }

    if(stmt->isNestedBlock()) {
        checkReachability((NestedBlock*) stmt);
    } else if(stmt->isIfStmt() || stmt->isIfThenElseStmt()) {
        checkReachability((IfStmt*) stmt);
    } else if(stmt->isWhileStmt()) {
        checkReachability((WhileStmt*) stmt);
    } else if(stmt->isForStmt()) {
    } else if(stmt->isReturnStmt()) {
    } else {
        // everything else i.e local variable, assignment stmt, method invocation stmt,
        // class creation stmt and empty statement
        checkCompletionOfPrevStmt(stmt);
    }
}

void Reachable::checkReachability(NestedBlock* block) {
    // only if it is not the last statement
    if(!checkCompletionOfPrevStmt(block)) {
        // something wrong happened, silently return
        return;
    }
   
    // if past here, then we know the block is reachable
    if(!block->isEmptyNestedBlock()) {
        // as long as it's not an empty block
        // In Joos all blocks can be reached, as long as the previous
        // statement can be completed. This is because there are no switch blocks
        BlockStmts* endStmt = block->getNestedBlock()->getStatements();
        checkReachability(endStmt);
        if(!endStmt->canComplete()) {
            // the last statement in the nested block cannot complete
            std::stringstream ss;
            ss << "Nested block statement cannot complete because the last statement in the block cannot complete.";
            Error(E_REACHABILITY, NULL, ss.str());

            // set reachability to false
            block->setReachability(false);
        }
    }
}

void Reachable::checkReachability(IfStmt* stmt) {
    if(!checkCompletionOfPrevStmt(stmt)) {
        return;
    }

    BlockStmts* truePart = stmt->getExecuteTruePart();
    BlockStmts* falsePart = stmt->getExecuteFalsePart();
    checkReachability(truePart);
    if(falsePart != NULL) {
        // if this is an if-then-else statement
        checkReachability(falsePart);
        // set the completion of the if-then-else statement
        stmt->setCompletion(truePart->canComplete() || falsePart->canComplete());
    }
}

void Reachable::checkReachability(WhileStmt* stmt) {
    if(!checkCompletionOfPrevStmt(stmt)) {
        return;
    }

    BlockStmts* loop = stmt->getLoopStmt();
    BOOL_CONST_EXPR_VAL constExprVal = checkConstExprAndEval(stmt->getExpressionToEvaluate());
    if(constExprVal == CE_TRUE) {
        // the expression to check is a constant expression that
        // evaluates to constant expression
        stmt->setCompletion(false);
    }

    // the contained statement is unreachable if the negation of this
    // expression is true
    unreachableStmt |= !(stmt->isReachable() && constExprVal != CE_FALSE);
    if(!unreachableStmt) {
        // if the contained stmt can be reached
        checkReachability(loop);
    } else {
        std::stringstream ss;
        ss << "Loop statement of while statement cannot be reached because the condition expression is a constant expression that evaluates to false.";
        Error(E_REACHABILITY, NULL, ss.str());
    }
}

void Reachable::checkReachability(Constructor* ctor) {
    inConstructor = true;
    checkReachability(ctor->getConstructorBody());
    // reset
    inConstructor = false;
}

// -----------------------------------------------------------------------
// error reporting

bool Reachable::checkCompletionOfPrevStmt(BlockStmts* stmt) {
    if(!stmt->isLastStatement()) {
        // only if it is not the last statement
        if(!stmt->getNextStatement()->canComplete()) {
            // the previous statement cannot be completed, therefore set
            // the currently seen stmt's reachability to false,
            // indicate some statement can't be reached
            // and emit an error
            
            stmt->setReachability(false);
            unreachableStmt |= true;
            std::string stmtKind;
            std::string cause = "a previous statement cannot be completed normally.";
            if(stmt->isLocalVarDecl()) {
                stmtKind = "Local variable declaration";
            } else if(stmt->isAssignStmt()) {
                stmtKind = "Assignment";
            } else if(stmt->isClassCreationStmt()) {
                stmtKind = "Creation of class";
            } else if(stmt->isMethodInvokeStmt()) {
                stmtKind = "Method invocation";
            } else if(stmt->isIfStmt()) {
                stmtKind = "If then";
            } else if(stmt->isIfThenElseStmt()) {
                stmtKind = "If then else";
            } else if(stmt->isWhileStmt()) {
                stmtKind = "While";
            } else if(stmt->isForStmt()) {
                stmtKind = "For";
            } else if(stmt->isReturnStmt()) {
                stmtKind = "Return";
            } else {
                // precuationary check
                assert(stmt->isEmptyStmt());
                stmtKind = "Empty";
            }
            reportUnreachableStatement(stmtKind, cause, NULL);
        }
        return false;
    }
    return true;
}

void Reachable::reportUnreachableStatement(const std::string& stmtKind, const std::string& cause, Token* tok) {
    std::stringstream ss;
    ss << stmtKind << " statement cannot be reached because " << cause << ".";
    Error(E_REACHABILITY, tok, ss.str());
}

// -----------------------------------------------------------------------
// checks whether an expression is a constant expression and evaluate its value
CONST_EXPR_VAL checkConstExprAndVal(Expression* expr) {
    // assumption is that expr is of type boolean coming from A3
    // precuationary check
    assert(expr->isExprTypeBoolean());
    if(expr->isRegularBinaryExpression()) {
        return checkConstExprAndVal((BinaryExpression*) expr);
    } else if(expr->isBooleanNegation() || expr->isNumericNegation()) {
        return checkConstExprAndVal((NegationExpression*) expr);
    } else if(expr->isPrimaryExpression()) {
        return checkConstExprAndVal((PrimaryExpression*) expr);
    }
    // anything else
    return CE_NONCONSTEXPR;
}

CONST_EXPR_VAL checkConstExprAndVal(BinaryExpression* expr) {
    Expression* leftExpr = expr->getLeftExpression();
    Expression* rightExpr = expr->getRightExpression();
    CONST_EXPR_VAL leftExprVal = checkConstExprAndVal(leftExpr);
    CONST_EXPR_VAL rightExprVal = checkConstExprAndVal(rightExpr);

    std::string leftConstExprVal = leftExpr->getNonBoolConstExpr();
    std::string rightConstExprVal = rightExpr->getNonBoolConstExpr();

    if(expr->isLazyOr() || expr->isEagerOr()) {
        // for || and | boolean operations

        // if the left expression evaluates to true or is not a constant expression
        // then return the left expression's value
        assert(leftExpr->isExprTypeBoolean() && rightExpr->isExprTypeBoolean());
        if(leftExprVal != CE_FALSE) { return leftExprVal; }
        else {
            // just return whatever the right expression evaluates to, even if it's
            // not a constant expression
            return rightExprVal;
        }
    } else if(expr->isLazyAnd() || expr->isEagerAnd()) {
        // for && and & boolean operations

        // if the left expression evaluates to false or is not a constant expression
        // then return the left expression's value
        // precautionary check
        assert(leftExpr->isExprTypeBoolean() && rightExpr->isExprTypeBoolean());
        if(leftExprVAL != CE_TRUE) { return leftExprVal;}
        else {
            // return the value of the right hand side
            return rightExprVal;
        }
    } else if(expr->isEqual()) {
        if(leftExprVal == CE_PRIMITIVEORSTRING || rightExprVal == CE_PRIMITIVEORSTRING) {
            // both sides referred to a literal of primitive types or a string literal
            if(leftExpr->getNonBoolConstExpr() == rightExpr->getNonBoolConstExpr()) {
                // they are the same, then equality comparison is true
                return CE_TRUE;
            } else { return CE_FALSE; }
        } else if((leftExprVal == CE_TRUE && rightExprVal == CE_TRUE) ||
                  (leftExprVal == CE_FALSE && rightExprVal == CE_FALSE)) {
            // both sides are false or true
            return CE_TRUE;
        } else if((leftExprVal == CE_TRUE && rightExprVal == CE_FALSE) ||
                  (leftExprVal == CE_FALSE && rightExprVal == CE_TRUE)) {
            // one side is true the other false
            return CE_FALSE;
        }
    } else if(expr->isNotEqual()) {
        // the same like above but flipped
        if(leftExprVal == CE_PRIMITIVEORSTRING || rightExprVal == CE_PRIMITIVEORSTRING) {
            if(leftExpr->getNonBoolConstExpr() == rightExpr->getNonBoolConstExpr()) {
                return CE_FALSE;
            } else { return CE_TRUE; }
        } else if((leftExprVal == CE_TRUE && rightExprVal == CE_TRUE) ||
                  (leftExprVal == CE_FALSE && rightExprVal == CE_FALSE)) {
            return CE_FALSE;
        } else if((leftExprVal == CE_TRUE && rightExprVal == CE_FALSE) ||
                  (leftExprVal == CE_FALSE && rightExprVal == CE_TRUE)) {
            return CE_TRUE;
        }
    } else if(expr->isLT() || expr->isLTE() || expr->isGT() || expr->isGTE()) {
        // assumption is that both sides are of primitive numeric type
        // precuationary check
        assert((leftExpr->isExprTypeInt() || leftExpr->isExprTypeChar()) &&
               (rightExpr->isExprTypeInt() || rightExpr->isExprTypeChar()));
        
        if(leftExprVal == CE_PRIMITIVEORSTRING && rightExprVal == CE_PRIMITIVEORSTRING) {
            // both sides are constant expressions of primitive numeric types
            int leftNum, rightNum;
            char leftChar, rightChar;
            
            convertConstantValueToIntOrChar(leftConstExprVal, leftNum, leftChar);
            convertConstantValueToIntOrChar(rightConstExprVal, rightNum, rightChar);

            // now do the checks on the integers, which always has the value
            if(expr->isLT()) {
                if(leftNum < rightNum) { return CE_TRUE; }
                else { return CE_FALSE; }
            } else if(expr->isLTE()) {
                if(leftNum <= rightNum) { return CE_TRUE; }
                else { return CE_FALSE; }
            } else if(expr->isGT()) {
                if(leftNum > rightNum) { return CE_TRUE; }
                else { return CE_FALSE; }
            } else {
                if(leftNum >= rightNum) { return CE_TRUE; }
                else { return CE_FALSE; }
            }
        }
    } else if(expr->isAddition()) {
        // assumption is that either both sides is of a primitive numeric type,
        // or at least one of them is a string

        if(leftExprVal == CE_PRIMITIVEORSTRING && rightExprVal == CE_PRIMITIVEORSTRING) {
            if(leftExpr->isExprTypeObject() || rightExpr->isExprTypeObject()) {
                // assumption is that they are both string literals
                // precautionary check
                if(leftExpr->isExprTypeObject()) { assert(leftExpr->getExpressionTypeString() == "java.lang.String"); }
                if(rightExpr->isExprTypeObject()) { assert(leftExpr->getExpressionTypeString() == "java.lang.String"); }

                expr->setNonBoolConstExpr(leftExprVal + rightExprVal);
            } else {
                // both of them must be primitive numeric types then (char literal or number)
                // precuationary check
                assert((leftExpr->isExprTypeInt() || leftExpr->isExprTypeChar()) &&
                       (rightExpr->isExprTypeInt() || rightExpr->isExprTypeChar()));

                std::stringstream ss;
                ss << evaluateBinaryNumericOperation(leftConstExprVal, rightConstExprVal, expr->getRule());
                expr->setNonBoolConstExpr(ss.str());
            }

            // one of the sides is not a constant expression
            return CE_PRIMITIVEORSTRING;
        }
    } else {
        // the rest of the binary expressions
        assert(expr->isMinus() || expr->isMultiplication() || expr->isModulo() || expr->isDivision());
        // both sides must be primitive numeric types then (by assumption)
        if(leftExprVal == CE_PRIMITIVEORSTRING && leftExprVal == CE_PRIMITIVEORSTRING) {
            // precautionary check
            assert((leftExpr->isExprTypeInt() || leftExpr->isExprTypeChar()) &&
                   (rightExpr->isExprTypeInt() || rightExpr->isExprTypeChar()));
            
            std::stringstream ss;
            ss << evaluateBinaryNumericOperation(leftConstExprVal, rightConstExprVal, expr->getRule());
            expr->setNonBoolConstExpr(ss.str());
        }
    }
    assert(leftExprVal == CE_NONCONSTEXPR || rightExprVal == CE_NONCONSTEXPR);
    return CE_NOTCONSTEXPR;
}

CONST_EXPR_VAL Reachable::checkConstExprAndVal(NegationExpression* expr) {
    Expression* negExpr = expr->getNegatedExpression();
    CONST_EXPR_VAL negVal = checkConstExprAndVal(negExpr);
    std::string negValString = expr->getNonBoolConstExpr();

    if(expr->isBooleanNegation()) {
        if(negVal == CE_TRUE) {
            return CE_FALSE;
        } else if(negVal == CE_FALSE) {
            return CE_TRUE;
        }
    } else {
        // numeric negation
        // expression must be of primitive numeric type
        if(negVal == CE_PRIMITIVEORSTRING) {
            // precautionary check
            assert(negExpr->isExprTypeInt() || negExpr->isExprTypeChar());
            int n; char c;
            convertConstantValue(negValString, n, c);
            std::stringstream ss;
            ss << -n;
            expr->setNonBoolConstExpr(ss.str());
        }
    }
    // the only possible value left
    assert(negVal == CE_NONCONSTEXPR);
    return negVal;
}

CONST_EXPR_VAL checkConstExprAndVal(PrimaryExpression* expr) {
    Primary* prim = expr->getPrimaryExpression();
    if(prim->isLiteralOrThis()) {
        LiteralOrThis* lit = (LiteralOrThis*) prim;
        if(lit->isNumber() || lit->isChar() || lit->isStringLiteral()) {
            expr->setNonBoolConstExpr(lit->getLiteralAsString());
            return CE_PRIMITIVEORSTRING;
        } else if(lit->isTrueBoolean()) {
            return CE_TRUE;
        } else if(lit->isFalseBoolean()) {
            return CE_FALSE;
        }
    }
    // anything else
    return CE_NONCONSTEXPR;
}

// -----------------------------------------------------------------------
// helper functions

void Reachable::convertConstantValueToIntOrChar(const std::string& constExprVal, int& n, char& c) {
    std::stringstream ss(constExprVal);
    if(ss >> n) {
    } else {
        // must be a char
        ss.clear();
        ss << constExprVal;
        // allows newline and the likes to be gotten
        ss >> std::noskipws >> c;
        // set the integer to the value of the char
        // too
        n = c;
    }
}

int Reachable::evaluateBinaryNumericOperation(const std::string& leftOp, const std::string& rightOp, RuleNumers ruleOp) {
    // assumption is that both leftOp and rightOp is a number
    int leftNum, rightNum;
    char leftChar, rightChar;
    convertConstantValueToIntOrChar(leftOp, leftNum, leftChar);
    convertConstantValueToIntOrChar(rightOp, rightNum, rigthChar);
    
    if(ruleOp == ADD_TO_PLUSMULTI) {
        return leftNum + rightNum;
    } else if(ruleOp == ADD_TO_MINUSMULTI) {
        return leftNum - rightNum;
    } else if(ruleOp == MULTI_TO_MULTUNARY) {
        return leftNum * rightNum;
    } else if(ruleOp == MULTI_TO_DIVUNARY) {
        return leftNum / rightNum;
    }
    // the last possible rule given
    assert(ruleOp == MULTI_TO_MODUNARY);
    return leftNum % rightNum;
}

// -----------------------------------------------------------------------
// Called by main

Reachable::checkReachability() {
    std::map<std::string, std::vector<CompilationTable*> >::iterator it;
    for(it = compilations.begin(); it != compilations.end(); it++) {
        std::vector<CompilationTable*>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            checkReachability(*it2);
            // reset
            unreachableStmt = false;
        }
    }
}
