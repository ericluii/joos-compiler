#ifndef __INTERFACEMETHOD_H__
#define __INTERFACEMETHOD_H__

#include "ast.h"
#include "modifiersStar.h"
#include "type.h"
#include "identifier.h"
#include "formalParamStar.h"

class InterfaceMethod : public Ast {
    // Rule: INTERFACE_MEMBER_DECL and INTERFACE_MEMBER_DECL_LIST
    private:
        ModifiersStar* mods;
        Type* retType;
        Identifier* id;
        FormalParamStar* params;
        InterfaceMethod* nextMethod;
    public:
        InterfaceMethod(ModifiersStar* mods, Type* retType, Identifier* id, FormalParamStar* params) :
            mods(mods), retType(retType), id(id), params(params), nextMethod(NULL) {}
        ~InterfaceMethod() {
            delete mods;
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
};

#endif
