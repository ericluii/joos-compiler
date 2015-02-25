#ifndef __INTERFACEBODYSTAR_H__
#define __INTERFACEBODYSTAR_H__

#include "ast.h"
#include "interfaceMethod.h"

class InterfaceBodyStar : public Ast {
    // Rule: INTERFACE_MEMBER_DECL_STAR and INTERFACE_MEMBER_DECL_EMPTY
    private:
        InterfaceMethod* methods;
    public:
        InterfaceBodyStar(InterfaceMethod* methods) : methods(methods) {}
        ~InterfaceBodyStar() {
            delete methods;
        }

        InterfaceMethod* getInterfaceMethods() { return methods; }
        bool isEpsilon() { return methods == NULL; }
};

#endif
