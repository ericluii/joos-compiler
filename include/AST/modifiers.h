#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <string>
#include "ast.h"
#include "token.h"

class Modifiers : public Ast {
    private:
        Modifiers* nextModifier;
        Token* mod;
    public:
        Modifiers(Token* mod) : nextModifier(NULL), mod(mod) {}
        ~Modifiers() {
            delete nextModifier;
        }

        std::string getCurrentModifier() { return mod->getString(); }
        Modifiers* getNextModifier() { return nextModifier; }

        void setNextModifier(Modifiers* set) { nextModifier = set; }
        bool isEpsilon() { return false; }
};

#endif
