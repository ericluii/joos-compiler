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
        std::string getQualifier() {
            if(nextName == NULL) {
                return "";
            }
            return nextName->getFullName();
        }

        std::string getFullName() {
            if(nextName == NULL) {
                return id->getIdAsString();
            }
            return nextName->getFullName() + "." + id->getIdAsString();
        }

        void setNextName(Name* set) { nextName = set; }
        bool lastPrefix() { return nextName == NULL; }
};

#endif
