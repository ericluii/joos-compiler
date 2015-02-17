#ifndef __ARGUMENTSSTAR_H__
#define __ARGUMENTSSTAR_H__

#include "ast.h"
#include "arguments.h"

class ArgumentsStar : public Ast
{
    private:
        Arguments* args;
    public:
        ArgumentsStar(Arguments* args) : args(args) {}
        ~ArgumentsStar() {
            delete args;
        }
        bool isEpsilon() { return args == NULL; }

        Arguments* getArgs() { return args; }
};

#endif
