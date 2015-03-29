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

        Identifier* getClassMethodId() { return name; }
        Type* getReturnType() { return retType; }
        FormalParamStar* getClassMethodParams() { return params; }

        bool isVoidReturnType() { return retType == NULL; }
        std::string methodSignatureAsString() {
            std::string signature = name->getIdAsString() + '(';
            if(!params->isEpsilon()) {
                signature+= params->getListOfParameters()->parametersAsString(',');
            }
            return signature + ')';
        }

        std::string labelizedMethodSignature() {
            std::string methodLabel = name->getIdAsString() + '#';
            if(!params->isEpsilon()) {
                methodLabel+= params->getListOfParameters()->parametersAsString('#');
            }
            return methodLabel + '#';
        }
};

#endif
