#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include "ast.h"
#include "expression.h"

class Arguments : public Ast
{
    private:
        Expression* selfExpr;
        Arguments* nextArgs;
    public:
        Arguments(Expression* selfExpr) : selfExpr(selfExpr), nextArgs(NULL) {}
        ~Arguments() {
            delete selfExpr;
            delete nextArgs;
        }

        Arguments* getNextArgs() { return nextArgs; }
        Expression* getSelfExpr() { return selfExpr; }

        void setNextArgs(Arguments* set) { nextArgs = set; }
};

#endif
