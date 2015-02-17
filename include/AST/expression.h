#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "ast.h"

class Expression : public Ast {
    public:
        Expression() {}
        virtual ~Expression() {}
};

#endif
