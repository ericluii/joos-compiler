#ifndef __LITERALORTHIS_H__
#define __LITERALORTHIS_H__

#include "primary.h"
#include "token.h"

class LiteralOrThis : public Primary {
    private:
        Token* litOrThis;
    public:
        LiteralOrThis(Token* tok) : litOrThis(tok) {}
        ~LiteralOrThis() {}

        std::string getLiteralValue() { return litOrThis->getString(); }
};


#endif
