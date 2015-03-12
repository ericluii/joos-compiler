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
        Expression* getSelfArgumentExpr() { return selfExpr; }

        void setNextArgs(Arguments* set) { nextArgs = set; }
        bool lastArgument() { return nextArgs == NULL; }

        std::string stringifyToType() {
            std::string ret = "";
            if(nextArgs != NULL) {
                ret+= nextArgs->stringifyToType() + ',';
            } else {
                return selfExpr->getExpressionTypeString();
            }

            return ret + selfExpr->getExpressionTypeString();
        }
};

#endif
