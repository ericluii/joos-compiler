#ifndef __BINARYEXPRESSION_H__
#define __BINARYEXPRESSION_H__

#include "expression.h"

class BinaryExpression : public Expression {
    // Rule: All expression binary operations i.e ||, &&, |, &, ==, !=, <, <=, >, >=,
    //       +, - (not the one from NEG_UNARY), *, / , %, except instanceof
    private:
        Expression* leftExpr;
        Expression* rightExpr;
    public:
        BinaryExpression(Expression* leftExpr, Expression* rightExpr) : leftExpr(leftExpr), rightExpr(rightExpr) {}
        ~BinaryExpression() {
            delete leftExpr;
            delete rightExpr;
        }

        Expression* getLeftExpression() { return leftExpr; }
        Expression* getRightExpression() { return rightExpr; }
};

#endif
