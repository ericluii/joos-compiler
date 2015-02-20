#ifndef __PRIMITIVETYPE_H__
#define __PRIMITIVETYPE_H__

#include "type.h"
#include "token.h"

class PrimitiveType : public Type {
    // Rule: all the primitive type derivations and ARRAY_PRIMITIVE
    private:
        Token* primitiveType;
    public:
        PrimitiveType(Token* primitiveType) : primitiveType(primitiveType) {}
        ~PrimitiveType() {}

        std::string getTypeAsString() { return primitiveType->getString(); }
        bool isEpsilon() { return false; }
};

#endif
