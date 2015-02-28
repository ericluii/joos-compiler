#ifndef __CLASSMETHOD_H__
#define __CLASSMETHOD_H__

#include "classBodyDecls.h"
#include "methodHeader.h"
#include "methodBody.h"
#include "classMethodTable.h"

class ClassMethod : public ClassBodyDecls {
    // Rule: METHOD_HEADER_AND_BODY
    private:
        MethodHeader* header;
        MethodBody* body;
        ClassMethodTable* table;
    public:
        ClassMethod(Modifiers* mod, MethodHeader* header, MethodBody* body) : ClassBodyDecls(mod),
                    header(header), body(body), table(NULL) {}
        ~ClassMethod() {
            delete header;
            delete body;
        }

        MethodHeader* getMethodHeader() { return header; }
        MethodBody* getMethodBody() { return body; }
        void setClassMethodTable(ClassMethodTable* set) { table = set; }
        ClassMethodTable* getClassMethodTable() { return table; }
};

#endif
