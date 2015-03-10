#ifndef __PRIMARYEXPRESSION_H__
#define __PRIMARYEXPRESSION_H__

#include "expression.h"

class PrimaryExpression : public Expression {
    // Rule: PRIMARY_UNARY
    private:
        Primary* expr;
    public:
        PrimaryExpression(Primary* expr) : Expression(), expr(expr) {}
        ~PrimaryExpression() {
            delete expr;
        }

        Primary* getPrimaryExpression() { return expr; }
};

#endif
