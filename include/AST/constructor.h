#ifndef __CONSTRUCTOR_H__
#define __CONSTRUCTOR_H__

#include "classBodyDecls.h"
#include "identifier.h"
#include "formalParamStar.h"
#include "blockStmtsStar.h"

class Constructor : public ClassBodyDecls {
    // Rule: CLASS_CONSTRUCTOR
    private:
        Identifier* id;
        FormalParamStar* params;
        BlockStmtsStar* body;
    public:
        Constructor(Modifiers* mod, Identifier* id, FormalParamStar* params, BlockStmtsStar* body) :
                ClassBodyDecls(mod), id(id), params(params), body(body) {}
        ~Constructor() {
            delete id;
            delete params;
            delete body;
        }

        Identifier* getConstructorId() { return id; }
        FormalParamStar* getConstructorParameters() { return params; }
        BlockStmtsStar* getConstructorBody() { return body; }

        bool emptyConstructorBody() { return body->isEpsilon(); }
        std::string constructorSignatureAsString() {
            std::string signature = "(";
            if(!params->isEpsilon()) {
                signature+= params->getListOfParameters()->parametersAsString();
            }
            return signature + ')';
        }
};

#endif
