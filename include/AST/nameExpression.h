#ifndef __NAMEEXPRESSION_H__
#define __NAMEEXPRESSION_H__

#include "expression.h"
#include "name.h"

class NameExpression : public Expression {
    // Rule: UNARY_NAME
    private:
        Name* expr;
    public:
        NameExpression(Name* expr) : Expression(), expr(expr) {}
        ~NameExpression() {
            delete expr;
        }

        Name* getNameExpression() { return expr; }
};

#endif
