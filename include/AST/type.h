#ifndef __TYPE_H__
#define __TYPE_H__

#include "ast.h"

class Type : public Ast {
    public:
        Type() {}
        virtual ~Type() {}
        bool isArrayType() { return rule == ARRAY_PRIMITIVE ||
                                    rule == ARRAY_NONPRIMITIVE; }
};

#endif
