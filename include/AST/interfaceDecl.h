#ifndef __INTERFACEDECL_H__
#define __INTERFACEDECL_H__

#include "typeDecl.h"
#include "modifiers.h"
#include "identifier.h"
#include "interfaceList.h"
#include "interfaceBodyStar.h"

class InterfaceDecl : public TypeDecl
{
    private:
        Modifiers* mods;
        Identifier* id;
        InterfaceList* extends;
        InterfaceBodyStar* body;
    public:
        InterfaceDecl(Modifiers* mods, Identifier* id, InterfaceList* extends, InterfaceBodyStar* body) : 
                        mods(mods), id(id), extends(extends), body(body) {} 
        ~InterfaceDecl() {
            delete mods;
            delete id;
            delete extends;
            delete body;
        }

        Modifiers* getInterfaceModifiers() { return mods; }
        Identifier* getInterfaceId() { return id; }
        InterfaceList* getExtendedInterfaces() { return extends; }
        InterfaceBodyStar* getInterfaceBody() { return body; }

        bool noExtendedInterfaces() { return extends->isEpsilon(); }
        bool emptyInterfaceBody() { return body->isEpsilon(); }
        bool isEpsilon() { return false; }
};

#endif
