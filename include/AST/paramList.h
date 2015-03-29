#ifndef __PARAMLIST_H__
#define __PARAMLIST_H__

#include "ast.h"
#include "type.h"
#include "identifier.h"

class ParamTable;

class ParamList : public Ast {
    // Rule: FORMAL_PARAM and FORMAL_PARAM_LIST
    private:
        Type* paramType;
        Identifier* paramId;
        ParamList* nextParam;
        ParamTable* table;
    public:
        ParamList(Type* paramType, Identifier* paramId) : paramType(paramType), paramId(paramId), nextParam(NULL), table(NULL) {}
        ~ParamList() {
            delete paramType;
            delete paramId;
            delete nextParam;
        }

        Type* getParameterType() { return paramType; }
        Identifier* getParameterId() { return paramId; }
        ParamList* getNextParameter() { return nextParam; }
        void setNextParameter(ParamList* set) { nextParam = set; }
        bool isLastParameter() { return nextParam == NULL; }

        std::string parametersAsString(const char& seperator) {
            std::string res = "";
            if(!isLastParameter()) {
                res+= nextParam->parametersAsString(seperator) + seperator;
            } else {
                if(seperator == ',') { return paramType->getTypeAsString(); }
                else { return paramType->getTypeAsString(true); }
            }

            if(seperator == ',') { return res + paramType->getTypeAsString(); }
            else { return res + paramType->getTypeAsString(true); }
        }

        void setParamTable(ParamTable* set) { table = set; }
        ParamTable* getParamTable() { return table; }
};

#endif
