#ifndef __CLASSDECL_H__
#define __CLASSDECL_H__

#include "typeDecl.h"
#include "modifiers.h"
#include "identifier.h"
#include "super.h"
#include "interfaceList.h"
#include "classBodyStar.h"
#include "classTable.h"

class ClassDecl : public TypeDecl
{
    // Rule: TYPE_CLASS
    private:
        Modifiers* mods;
        Identifier* id;
        Super* super;
        InterfaceList* interfaces;
        ClassBodyStar* body;
        ClassTable* table;
    public:
        ClassDecl(Modifiers* mods, Identifier* id, Super* super, InterfaceList* interfaces, ClassBodyStar* body) 
            : mods(mods), id(id), super(super), interfaces(interfaces), body(body), table(NULL) {}
        ~ClassDecl() {
            delete mods;
            delete id;
            delete super;
            delete interfaces;
            delete body;
        }

        Modifiers* getClassModifiers() { return mods; }
        Identifier* getClassId() { return id; }
        Super* getSuper() { return super; }
        InterfaceList* getImplementInterfaces() { return interfaces; }
        ClassBodyStar* getClassMembers() { return body; }

        bool isAbstract() {
            Modifiers* m = getClassModifiers();

            while (m != NULL) {
                if (m->getCurrentModifierAsString() == "abstract") {
                    return true;
                }

                m = m->getNextModifier();
            }

            return false;
        }

        bool isProtected() {
            Modifiers* m = getClassModifiers();

            while (m != NULL) {
                if (m->getCurrentModifierAsString() == "protected") {
                    return true;
                }

                m = m->getNextModifier();
            }

            return false;
        }

        bool noSuperClass() { return super->isEpsilon(); }
        bool noImplementedInterfaces() { return interfaces->isEpsilon(); }
        bool emptyBody() { return body->isEpsilon(); }
        bool isEpsilon() { return false; }
        void setClassTable(ClassTable* set) { table = set; }
        ClassTable* getClassTable() { return table; }
};

#endif
