#ifndef __CLASSBODYDECLS_H__
#define __CLASSBODYDECLS_H__

#include "ast.h"
#include "modifiers.h"

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
};

#endif