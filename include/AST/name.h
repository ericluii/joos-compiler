#ifndef __NAMEDECL_H__
#define __NAMEDECL_H__

#include "ast.h"
#include "identifier.h"

class Name : public Ast
{
        Name *nextName;
        Identifier *id;
    public:
        Name(Name *nextName, Identifier *id) : nextName(nextName), id(id) {}
        Name(Identifier *id): nextName(NULL), id(id) {}

        Identifier *GetId() { return id; }
        Name *GetNextName() { return nextName; }

        void setNextName(Name* set) { nextName = set; }
};

#endif
