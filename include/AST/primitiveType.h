#ifndef __PRIMITIVETYPE_H__
#define __PRIMITIVETYPE_H__

#include <string>
#include "type.h"
#include "token.h"

class PrimitiveType : public Type {
    private:
        Token* primitiveType;
    public:
        PrimitiveType(Token* primitiveType) : primitiveType(primitiveType) {}
        ~PrimitiveType() {}

        std::string getTypeString() { return primitiveType->getString(); }
};

#endif
