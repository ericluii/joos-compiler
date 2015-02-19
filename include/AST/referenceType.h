#ifndef __REFERENCETYPE_H__
#define __REFERENCETYPE_H__

#include "type.h"
#include "name.h"

class ReferenceType : public Type {
    // Rule: REFERENCE_CLASSINTERFACE, and ARRAY_NONPRIMITIVE
    private:
        Name* referenceType;
    public:
        ReferenceType(Name* referenceType) : referenceType(referenceType) {}
        ~ReferenceType() {
            delete referenceType;
        }

        Name* getReferenceName() { return referenceType; }
};

#endif
