#ifndef __CLASSMETHOD_H__
#define __CLASSMETHOD_H__

#include <string>
#include "classBodyDecls.h"
#include "identifier.h"
#include "methodBody.h"

class ClassMethod : public ClassBodyDecls {
    private:
        Identifier* name;
        MethodBody* body;
    public:
        ClassMethod(Modifiers* mod, Identifier* name, MethodBody* body) : ClassBodyDecls(mod), name(name), body(body) {}
        ~ClassMethod() {
            delete name;
            delete body;
        }

        std::string getMethodName() { return name->token->getString(); }
        MethodBody* getMethodBody() { return body; }

};

#endif
