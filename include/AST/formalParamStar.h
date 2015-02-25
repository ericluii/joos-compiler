#ifndef __FORMALPARAMSTAR_H__
#define __FORMALPARAMSTAR_H__

#include "ast.h"
#include "paramList.h"

class FormalParamStar : public Ast {
    // Rule: FORMAL_PARAMSTAR and FORMAL_PARAMSTAR_EPSILON
    private:
        ParamList* params;
    public:
        FormalParamStar(ParamList* params) : params(params) {}
        ~FormalParamStar() {
            delete params;
        }

        ParamList* getListOfParameters() { return params; }
        bool isEpsilon() { return params == NULL; }
};

#endif
