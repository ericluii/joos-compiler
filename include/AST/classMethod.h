#ifndef __CLASSMETHOD_H__
#define __CLASSMETHOD_H__

#include "classBodyDecls.h"
#include "methodHeader.h"
#include "methodBody.h"

class ClassMethod : public ClassBodyDecls {
    // Rule: METHOD_HEADER_AND_BODY
    private:
        MethodHeader* header;
        MethodBody* body;
    public:
        ClassMethod(Modifiers* mod, MethodHeader* header, MethodBody* body) : ClassBodyDecls(mod),
                    header(header), body(body) {}
        ~ClassMethod() {
            delete header;
            delete body;
        }

        MethodHeader* getMethodHeader() { return header; }
        MethodBody* getMethodBody() { return body; }
};

#endif
