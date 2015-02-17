#ifndef __METHODNORMALINVOKE_H__
#define __METHODNORMALINVOKE_H__

#include "methodInvoke.h"
#include "name.h"

class MethodNormalInvoke : public MethodInvoke {
    private:
        Name* methodName;
    public:
        MethodNormalInvoke(Name* methodName, ArgumentsStar* args) : MethodInvoke(args), methodName(methodName) {}
        ~MethodNormalInvoke() {
            delete methodName;
        }

        Name* getMethodName() { return methodName; }
};

#endif
