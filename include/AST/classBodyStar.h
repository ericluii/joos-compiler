#ifndef __CLASSBODYSTAR_H__
#define __CLASSBODYSTAR_H__

#include "ast.h"
#include "classBodyDecls.h"

class ClassBodyStar : public Ast {
    private:
        ClassBodyDecls* body;
    public:
        ClassBodyStar(ClassBodyDecls* body) : body(body) {}
        ~ClassBodyStar() {
            if(body != NULL) {
                delete body;
            }
        }

        bool isEpsilon() { return body == NULL; }
        ClassBodyDecls* getBody() { return body; }
};

#endif
