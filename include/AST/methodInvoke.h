#ifndef __METHODINVOKE_H__
#define __METHODINVOKE_H__

#include "primary.h"
#include "argumentsStar.h"

class MethodInvoke : public Primary {
    protected:
        ArgumentsStar* args;
    public:
        MethodInvoke(ArgumentsStar* args) : args(args) {}
        virtual ~MethodInvoke() {
            delete args;
        }

        ArgumentsStar* getInvokeArgsStar() { return args; }
};

#endif
