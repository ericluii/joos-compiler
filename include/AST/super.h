#ifndef __SUPER_H__
#define __SUPER_H__

#include "ast.h"
#include "name.h"

class Super : public Ast {
    // Rule: EXTENDS_CLASS || EXTENDS_CLASS_EPSILON
    private:
        Name* superClass;
    public:
        Super(Name* superClass) : superClass(superClass) {}
        ~Super() {
            delete superClass;
        }

        Name* getSuper() { return superClass; }
        bool isEpsilon() { return superClass == NULL; }
};

#endif
