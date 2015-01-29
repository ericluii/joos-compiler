#ifndef __BANNEDKEYWORDDFA_H__
#define __BANNEDKEYWORDDFA_H__

#include "dfa.h"

class BannedKeywordDfa : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        BannedKeywordDfa();
};

#endif
