#ifndef __FIELDACCESS_H__
#define __FIELDACCESS_H__

#include "ast.h"
#include "primary.h"
#include "identifier.h"

class FieldAccess : public Primary {
    // Rule: FIELD_ACCESS
    private:
        Primary* primary;
        Identifier* id;
    public:
        FieldAccess(Identifier* id) : primary(NULL), id(id) {}
        ~FieldAccess() {
            delete primary;
            delete id;
        }

        Primary* getAccessedFieldPrimary() { return primary; }
        Identifier* getAccessedFieldId() { return id; }

        void setPrimaryField(Primary* set) { primary = set; }
};

#endif
