#ifndef __IMPLEMENTS_H__
#define __IMPLEMENTS_H__

#include "ast.h"
#include "name.h"

class Implements : public Ast {
    private:
        Name* interface;
        Implements* nextImplement;
    public:
        Implements(Name* interface) : interface(interface), nextImplement(NULL) {}
        ~Implements() {
            delete interface;
            delete nextImplement;
        }

        Name* getInterface() { return interface; }
        Implements* getNextImplementedInterface() { return nextImplement; }
        void setNextImplement(Implements* set) { nextImplement = set; }
};

#endif
