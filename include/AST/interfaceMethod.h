#ifndef __INTERFACEMETHOD_H__
#define __INTERFACEMETHOD_H__

#include "ast.h"
#include "modifiersStar.h"
#include "type.h"
#include "identifier.h"
#include "formalParamStar.h"
#include "interfaceMethodTable.h"
#include <iostream>

class InterfaceMethod : public Ast {
    // Rule: INTERFACE_MEMBER_DECL and INTERFACE_MEMBER_DECL_LIST
    private:
        ModifiersStar* mods;
        Type* retType;
        Identifier* id;
        FormalParamStar* params;
        InterfaceMethod* nextMethod;
        InterfaceMethodTable* table;
    public:
        InterfaceMethod(ModifiersStar* mods, Type* retType, Identifier* id, FormalParamStar* params) :
            mods(mods), retType(retType), id(id), params(params), nextMethod(NULL), table(NULL) {}
        ~InterfaceMethod() {
            delete mods;
            delete retType;
            delete id;
            delete params;
            delete nextMethod;
        }

        ModifiersStar* getModifiersStar() { return mods; }
        Type* getReturnType() { return retType; }
        Identifier* getInterfaceMethodId() { return id; }
        FormalParamStar* getParametersList() { return params; }
        InterfaceMethod* getNextInterfaceMethod() { return nextMethod; }
        
        void setNextInterfaceMethod(InterfaceMethod* set) { nextMethod = set; }
        bool isLastMethod() { return nextMethod == NULL; }
        bool noModifiers() { return mods->isEpsilon(); }
        bool isVoidReturnType() { return retType == NULL; }
        bool noParameters() { return params->isEpsilon(); }

        std::string methodSignatureAsString() {
            std::string signature = id->getIdAsString() + '(';
            if(!params->isEpsilon()) {
                signature+= params->getListOfParameters()->parametersAsString();
            }
            return signature + ')';
        }

        //declaring an interface method to be static should cause an error
        bool isStatic() {
            ModifiersStar* ms = getModifiersStar();

            if (!ms->isEpsilon()) {
                Modifiers* m = ms->getListOfModifiers();

                while (m != NULL) {
                    if (m->getCurrentModifierAsString() == "static") {
                        return true;
                    }

                    m = m->getNextModifier();
                }
            }

            return false;
        }

        bool isProtected() {
            ModifiersStar* ms = getModifiersStar();

            if (!ms->isEpsilon()) {
                Modifiers* m = ms->getListOfModifiers();

                while (m != NULL) {
                    if (m->getCurrentModifierAsString() == "protected") {
                        return true;
                    }

                    m = m->getNextModifier();
                }
            }

            return false;
        }
        
        //declaring an interface method to be final should cause an error
        bool isFinal() {
            ModifiersStar* ms = getModifiersStar();

            if (!ms->isEpsilon()) {
                Modifiers* m = ms->getListOfModifiers();

                while (m != NULL) {
                    if (m->getCurrentModifierAsString() == "final") {
                        return true;
                    }

                    m = m->getNextModifier();
                }
            }

            return false;
        }

        void setInterfaceMethodTable(InterfaceMethodTable* set) { table = set; }
        InterfaceMethodTable* getInterfaceMethodTable() { return table; }
};

#endif
