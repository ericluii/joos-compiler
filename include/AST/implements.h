#ifndef __IMPLEMENTS_H__
#define __IMPLEMENTS_H__

#include "ast.h"
#include "name.h"

class Implements : public Ast {
    private:
        Name* implements;
    public:
        Implements(Name* implements) : implements(implements) {}
        ~Implements() {
            delete implements;
        }

        bool isEpsilon() { return implements == NULL; }

        Name* getImplement() { return implements; }
};

#endif
