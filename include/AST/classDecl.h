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

        bool checkForModifier(const std::string& lookFor) {
            Modifiers* m = mods;
            while(m != NULL) {
                if(m->getCurrentModifierAsString() == lookFor) {
                    return true;
                }
                m = m->getNextModifier();
            }
            return false;
        }

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
            return checkForModifier("abstract");
        }

        bool isProtected() {
            return checkForModifier("protected");
        }

        bool isFinal() {
            return checkForModifier("final");
        }

        bool noSuperClass() { return super->isEpsilon(); }
        bool noImplementedInterfaces() { return interfaces->isEpsilon(); }
        bool emptyBody() { return body->isEpsilon(); }
        bool isEpsilon() { return false; }
        void setClassTable(ClassTable* set) { table = set; }
        ClassTable* getClassTable() { return table; }
};

#endif
