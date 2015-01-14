#ifndef __IDENTIFIERDFA_H__
#define __IDENTIFIERDFA_H__

#include "dfa.h"

class IdentifierDfa final : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        IdentifierDfa();
};

#endif
