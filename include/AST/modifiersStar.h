#ifndef __MODIFIERSSTAR_H__
#define __MODIFIERSSTAR_H__

#include "ast.h"
#include "modifiers.h"

class ModifiersStar : public Ast {
    private:
        Modifiers* mods;
    public:
        ModifiersStar(Modifiers* mods) : mods(mods) {}
        ~ModifiersStar() {
            delete mods;
        }

        virtual bool isEpsilon() { return mods == NULL; }
        Modifiers* getModifiers() { return mods; }
};

#endif
