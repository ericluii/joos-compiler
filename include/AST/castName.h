#ifndef __CASTNAME_H__
#define __CASTNAME_H__

#include "castExpression.h"
#include "name.h"

class CompilationTable;

class CastName : public CastExpression {
    // Rule: CAST_NONPRIMITIVE and CAST_TO_EXPRESSION
    private:
        Name* castType;
        CompilationTable* castTable;
    public:
        CastName(Name* castType, Expression* toCast) : CastExpression(toCast), castType(castType), castTable(NULL) {}
        ~CastName() {
            delete castType;
        }

        Name* getNameToCastTo() { return castType; }
        void setCastTypeTable(CompilationTable* set) { castTable = set; }
        CompilationTable* getCastTypeTable() { return castTable; }
        std::string getTypeToCastAsString() {
            std::string ret;
            if(castTable != NULL) {
                ret = castTable->getCanonicalName();
            } else {
                ret = castType->getFullName();
            }

            if(isCastToArrayName()) {
                return ret+"[]";
            }
            return ret;
        }
};

#endif
