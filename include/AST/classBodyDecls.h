#ifndef __CLASSBODYDECLS_H__
#define __CLASSBODYDECLS_H__

#include "ast.h"

class ClassBodyDecls : public Ast {
    private:
        ClassBodyDecls* declaration;
        ClassBodyDecls* nextDeclaration;
    public:
        ClassBodyDecls() {}
        bool isEpsilon() { return false; }

        ClassBodyDecls* getDeclaration() { return declaration; }
        ClassBodyDecls* getNextDeclaration() { return nextDeclaration; }
};

#endif
