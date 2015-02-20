#ifndef __CLASSDECL_H__
#define __CLASSDECL_H__

#include "typeDecl.h"
#include "modifiers.h"
#include "identifier.h"
#include "super.h"
#include "interfaceList.h"
#include "classBodyStar.h"

class ClassDecl : public TypeDecl
{
    // Rule: TYPE_CLASS
    private:
        Modifiers* mods;
        Identifier* id;
        Super* super;
        InterfaceList* interfaces;
        ClassBodyStar* body;
    public:
        ClassDecl(Modifiers* mods, Identifier* id, Super* super, InterfaceList* interfaces, ClassBodyStar* body) 
            : mods(mods), id(id), super(super), interfaces(interfaces), body(body) {}
        ~ClassDecl() {
            delete mods;
            delete id;
            delete super;
            delete interfaces;
            delete body;
        }

        Modifiers* getModifiers() { return mods; }
        Identifier* getIdentifier() { return id; }
        Super* getSuper() { return super; }
        InterfaceList* getListOfInterfaces() { return interfaces; }
        ClassBodyStar* getClassMembers() { return body; }

        bool noSuperClass() { return super->isEpsilon(); }
        bool noImplementedInterfaces() { return interfaces->isEpsilon(); }
        bool emptyBody() { return body->isEpsilon(); }
        bool isEpsilon() { return false; }
};

#endif
