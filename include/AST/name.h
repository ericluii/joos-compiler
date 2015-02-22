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

        Identifier *getNameId() { return id; }
        Name *getNextName() { return nextName; }
        std::string getFullName() {
            if(rule == NAME_SIMPLE) {
                return id->getIdAsString();
            }

            std::string qualifiedName = id->getIdAsString();
            Name* nextName = this->nextName;
            while(true) {
                switch(nextName->rule) {
                    case NAME_QUALIFIED:
                        qualifiedName = nextName->id->getIdAsString() + "." + qualifiedName;
                        nextName = nextName->nextName;
                        break;
                    case NAME_SIMPLE:
                        return nextName->id->getIdAsString() + "." + qualifiedName;
                }
            }
        }

        void setNextName(Name* set) { nextName = set; }
        bool lastPrefix() { return nextName == NULL; }
};

#endif
