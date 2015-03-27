#ifndef __TYPE_H__
#define __TYPE_H__

#include "ast.h"

class Type : public Ast {
    public:
        Type() {}
        virtual ~Type() {}
        virtual std::string getTypeAsString(bool labelForm = false) = 0;
};

#endif
