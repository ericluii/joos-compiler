#ifndef __SUPER_H__
#define __SUPER_H__

#include "ast.h"
#include "name.h"

class Super : public Ast {
    private:
        Name* superClass;
    public:
        Super(Name* superClass) : superClass(superClass) {}
        bool isEpsilon() { return superClass == NULL; }

        Name* getSuper() { return superClass; }
};

#endif
