#ifndef __PRIMITIVETYPE_H__
#define __PRIMITIVETYPE_H__

#include "type.h"
#include "token.h"

class PrimitiveType : public Type {
    // Rule: all the primitive type derivations and ARRAY_PRIMITIVE
    private:
        Token* primitiveType;
        bool isArray;
    public:
        PrimitiveType(Token* primitiveType, bool isArray) : primitiveType(primitiveType), isArray(isArray) {}
        ~PrimitiveType() {}

        std::string getTypeAsString() {
            std::string type = primitiveType->getString();
            if(isArray) {
                return type + "[]";
            }
            return type;
        }

        bool isPrimitiveArray() { return isArray; }
};

#endif
