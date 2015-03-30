#ifndef __CLASSBODYDECLS_H__
#define __CLASSBODYDECLS_H__

#include <string>
#include "ast.h"
#include "modifiers.h"

class ClassBodyDecls : public Ast {
    // Rule: CLASS_BODY_DECL and CLASS_BODY_LIST
    private:
        bool checkForModifier(const std::string& lookFor) {
            Modifiers* m = mod;
            while(m != NULL) {
                if(m->getCurrentModifierAsString() == lookFor) {
                    return true;
                }
                m = m->getNextModifier();
            }
            return false;
        }

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
            return checkForModifier("static");
        }
        
        bool isProtected() {
            return checkForModifier("protected");
        }

        bool isAbstract() {
            return checkForModifier("abstract");
        }
        
        bool isFinal() {
            return checkForModifier("final");
        }
};

#endif
