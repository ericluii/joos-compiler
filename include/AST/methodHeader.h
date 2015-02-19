#ifndef __METHODHEADER_H__
#define __METHODHEADER_H__

#include "ast.h"
#include "identifier.h"
#include "type.h"
#include "formalParamStar.h"

class MethodHeader : public Ast {
    // Rule: METHOD_TYPE and METHOD_VOID
    private:
        Identifier* name;
        Type* retType;
        FormalParamStar* params;
    public:
        MethodHeader(Identifier* name, Type* retType, FormalParamStar* params) : name(name),
                     retType(retType), params(params) {}
        ~MethodHeader() {
            delete name;
            delete retType;
            delete params;
        }

        Identifier* getMethodName() { return name; }
        Type* getReturnType() { return retType; }
        FormalParamStar* getParameters() { return params; }

        bool isVoidReturnType() { return retType == NULL; }
};

#endif
