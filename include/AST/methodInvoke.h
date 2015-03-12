#ifndef __METHODINVOKE_H__
#define __METHODINVOKE_H__

#include "primary.h"
#include "argumentsStar.h"

class MethodInvoke : public Primary {
    protected:
        ArgumentsStar* args;
    public:
        MethodInvoke(ArgumentsStar* args) : Primary(), args(args) {}
        virtual ~MethodInvoke() {
            delete args;
        }

        ArgumentsStar* getArgsForInvokedMethod() { return args; }
        bool noArgumentsGiven() { return args->isEpsilon(); }

        virtual std::string methodInvocationMatchToSignature() = 0;
};

#endif
