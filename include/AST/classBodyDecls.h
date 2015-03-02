#ifndef __CLASSBODYDECLS_H__
#define __CLASSBODYDECLS_H__

#include "ast.h"
#include "modifiers.h"
#include <iostream>

class ClassBodyDecls : public Ast {
    // Rule: CLASS_BODY_DECL and CLASS_BODY_LIST
    protected:
        Modifiers* mod;
        ClassBodyDecls* nextDeclaration;
    public:
        ClassBodyDecls(Modifiers* mod) :
                       mod(mod), nextDeclaration(NULL) {}
        
        virtual ~ClassBodyDecls() {
            delete mod;
            delete nextDeclaration;
        }

        ClassBodyDecls* getNextDeclaration() { return nextDeclaration; }
        Modifiers* getModifiers() { return mod; }
        void setNextDeclaration(ClassBodyDecls* set) { nextDeclaration = set; }
        bool isLastClassMember() { return nextDeclaration == NULL; }

        bool isStatic() {
            Modifiers* m = getModifiers();

            while (m != NULL) {
                if (m->getCurrentModifierAsString() == "static") {
                    return true;
                }

                m = m->getNextModifier();
            }

            return false;
        }
};

#endif
