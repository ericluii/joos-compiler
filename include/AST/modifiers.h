#ifndef __MODIFIERS_H__
#define __MODIFIERS_H__

#include <string>
#include "ast.h"
#include "token.h"

class Modifiers : public Ast {
    // Rule: 
    private:
        Modifiers* nextModifier;
        Token* mod;
    public:
        Modifiers(Token* mod) : nextModifier(NULL), mod(mod) {}
        ~Modifiers() {
            delete nextModifier;
        }

        std::string getCurrentModifierAsString() { return mod->getString(); }
        Modifiers* getNextModifier() { return nextModifier; }

        void setNextModifier(Modifiers* set) { nextModifier = set; }
        bool isLastModifier() { return nextModifier == NULL; }
};

#endif
