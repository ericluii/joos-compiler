#ifndef __INTERFACEDECL_H__
#define __INTERFACEDECL_H__

#include "typeDecl.h"
#include "modifiers.h"
#include "identifier.h"
#include "interfaceList.h"
#include "interfaceBodyStar.h"
#include "interfaceTable.h"

class InterfaceDecl : public TypeDecl
{
    private:
        Modifiers* mods;
        Identifier* id;
        InterfaceList* extends;
        InterfaceBodyStar* body;
        InterfaceTable* table;
    public:
        InterfaceDecl(Modifiers* mods, Identifier* id, InterfaceList* extends, InterfaceBodyStar* body) : 
                        mods(mods), id(id), extends(extends), body(body), table(NULL) {} 
        ~InterfaceDecl() {
            delete mods;
            delete id;
            delete extends;
            delete body;
        }

        Modifiers* getInterfaceModifiers() { return mods; }
        Identifier* getInterfaceId() { return id; }
        InterfaceList* getExtendedInterfaces() { return extends; }
        InterfaceBodyStar* getInterfaceBodyStar() { return body; }

        bool isFinal() {
            Modifiers* m = getInterfaceModifiers();

            while (m != NULL) {
                if (m->getCurrentModifierAsString() == "final") {
                    return true;
                }

                m = m->getNextModifier();
            }

            return false;
        }

        bool noExtendedInterfaces() { return extends->isEpsilon(); }
        bool emptyInterfaceBody() { return body->isEpsilon(); }
        bool isEpsilon() { return false; }
        void setInterfaceTable(InterfaceTable* set) { table = set; }
        InterfaceTable* getInterfaceTable() { return table; }
};

#endif
