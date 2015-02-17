#ifndef __ASSIGNMENT_H__
#define __ASSIGNMENT_H__

#include "expression.h"

class Assignment : public Expression {
    protected:
        Expression* RHS;
    public:
        Assignment(Expression* RHS) : RHS(RHS) {}
        virtual ~Assignment() {
            delete RHS;
        }

        Expression* getRHSExpression() { return RHS; }
};

#endif
