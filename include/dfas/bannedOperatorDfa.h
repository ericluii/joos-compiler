#ifndef __BANNEDOPERATORFA_H__
#define __BANNEDOPERATORFA_H__

#include "dfa.h"

class BannedOperatorDfa : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        static TOKEN_TYPE endTokenType;
        BannedOperatorDfa();
};

#endif
