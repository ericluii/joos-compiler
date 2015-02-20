#ifndef __CLASSBODYSTAR_H__
#define __CLASSBODYSTAR_H__

#include "ast.h"
#include "classBodyDecls.h"

class ClassBodyStar : public Ast {
    // Rule: CLASS_BODY_DECLSTAR and CLASS_BODY_DECL_EPSILON
    private:
        ClassBodyDecls* body;
    public:
        ClassBodyStar(ClassBodyDecls* body) : body(body) {}
        ~ClassBodyStar() {
            delete body;
        }

        bool isEpsilon() { return body == NULL; }
        ClassBodyDecls* getBody() { return body; }
};

#endif
