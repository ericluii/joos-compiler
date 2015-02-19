#ifndef __LITERALORTHIS_H__
#define __LITERALORTHIS_H__

#include "primary.h"
#include "token.h"

class LiteralOrThis : public Primary {
    // Rule: PRIMARY_THIS and LITERAL_NUM to LITERAL_NULL
    private:
        Token* litOrThis;
    public:
        LiteralOrThis(Token* tok) : litOrThis(tok) {}
        ~LiteralOrThis() {}

        std::string getLiteralValue() { return litOrThis->getString(); }
};


#endif
