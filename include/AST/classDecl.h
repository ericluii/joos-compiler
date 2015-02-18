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
    private:
        Modifiers* mods;
        Identifier* id;
        Super* super;
        InterfaceList* interfaces;
        ClassBodyStar* body;
        //fields
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
        ClassBodyStar* getClassBodyStar() { return body; }
        //get fields
        bool isEpsilon() { return false; }
};

#endif
