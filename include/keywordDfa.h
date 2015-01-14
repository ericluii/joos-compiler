#ifndef __KEYWORDDFA_H__
#define __KEYWORDDFA_H__

#include "dfa.h"

class KeywordDfa : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        KeywordDfa();
};

#endif
