#ifndef __MODIFIERSSTAR_H__
#define __MODIFIERSSTAR_H__

#include "ast.h"

class ModifiersStar : public Ast {
    public:
        ModifiersStar() {}
        virtual bool isEpsilon() { return true; }
};

#endif
