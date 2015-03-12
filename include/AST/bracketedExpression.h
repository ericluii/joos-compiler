#ifndef __BRACKETEDEXPRESSION_H__
#define __BRACKETEDEXPRESSION_H__

#include "primary.h"
#include "expression.h"

class BracketedExpression : public Primary {
    // Rule: PRIMARY_EXPRESSION
    private:
        Expression* expr;
    public:
        BracketedExpression(Expression* expr) : Primary(), expr(expr) {}
        ~BracketedExpression() {
            delete expr;
        }

        Expression* getExpressionInside() { return expr; }
};


#endif
