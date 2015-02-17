#ifndef __CLASSDECL_H__
#define __CLASSDECL_H__

#include "typeDecl.h"
#include "modifiers.h"
#include "identifier.h"
#include "super.h"
#include "implements.h"
#include "classBodyStar.h"

class ClassDecl : public TypeDecl
{
    private:
        Modifiers* mods;
        Identifier* id;
        Super* super;
        Implements* implementInterfaces;
        ClassBodyStar* body;
        //fields
    public:
        ClassDecl(Modifiers* mods, Identifier* id, Super* super, Implements* implements, ClassBodyStar* body) 
            : mods(mods), id(id), super(super), implementInterfaces(implements), body(body) {}
        ~ClassDecl() {
            delete mods;
            delete id;
            delete super;
            delete implementInterfaces;
            delete body;
        }

        Modifiers* getModifiers() { return mods; }
        Identifier* getIdentifier() { return id; }
        Super* getSuper() { return super; }
        Implements* getImplements() { return implementInterfaces; }
        ClassBodyStar* getClassBodyStar() { return body; }
        //get fields
        bool isEpsilon() { return false; }
};

#endif
