#ifndef __NAMEDECL_H__
#define __NAMEDECL_H__

#include "ast.h"
#include "identifier.h"

class Name : public Ast
{
    // Rule: NAME_SIMPLE and NAME_QUALIFIED
    private:
        Name *nextName;
        Identifier *id;
    public:
        Name(Identifier *id): nextName(NULL), id(id) {}
        ~Name() {
            delete id;
            delete nextName;
        }

        Identifier *GetId() { return id; }
        Name *GetNextName() { return nextName; }

        void setNextName(Name* set) { nextName = set; }
};

#endif
