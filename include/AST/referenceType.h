#ifndef __REFERENCETYPE_H__
#define __REFERENCETYPE_H__

#include "type.h"
#include "name.h"

class CompilationTable;

class ReferenceType : public Type {
    // Rule: REFERENCE_CLASSINTERFACE, and ARRAY_NONPRIMITIVE
    private:
        Name* referenceType;
        CompilationTable* referenceTable;
    public:
        ReferenceType(Name* referenceType) : referenceType(referenceType) {}
        ~ReferenceType() {
            delete referenceType;
        }

        Name* getReferenceName() { return referenceType; }
        std::string getTypeAsString() {
            std::string typeName = referenceType->getFullName();
            if(isReferenceArrayType()) {
                return typeName + "[]";
            }
            return typeName;
        }

        void setReferenceTypeTable(CompilationTable* set) { referenceTable = set; }
        CompilationTable* getReferenceTypeTable() { return referenceTable; }
};

#endif
