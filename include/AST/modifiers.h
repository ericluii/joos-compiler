#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <string>
#include "ast.h"
#include "token.h"
#include "modifiersStar.h"

class Modifiers : public ModifiersStar {
    private:
        Modifiers* nextModifier;
        Token* mod;
    public:
        Modifiers(Token* mod) : nextModifier(NULL), mod(mod) {}
        std::string getModifier() { return mod->getString(); }
        Modifiers* getNextModifier() { return nextModifier; }

        void setNextModifier(Modifiers* set) { nextModifier = set; }
        bool isEpsilon() { return true; }
};

#endif
