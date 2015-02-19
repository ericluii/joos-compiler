#ifndef __CLASSMETHOD_H__
#define __CLASSMETHOD_H__

#include <string>
#include "classBodyDecls.h"
#include "identifier.h"
#include "type.h"
#include "methodBody.h"

class ClassMethod : public ClassBodyDecls {
    // Rule: METHOD_TYPE and METHOD_VOID
    private:
        Identifier* name;
        Type* retType;
        MethodBody* body;
    public:
        ClassMethod(Modifiers* mod, Type* retType, Identifier* name, MethodBody* body) : ClassBodyDecls(mod),
                    name(name), retType(retType), body(body) {}
        ~ClassMethod() {
            delete name;
            delete retType;
            delete body;
        }

        std::string getMethodName() { return name->token->getString(); }
        Type* getReturnType() { return retType; }
        MethodBody* getMethodBody() { return body; }

        bool isVoidReturnType() { return retType == NULL; }
};

#endif
