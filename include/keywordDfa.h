#ifndef __KEYWORDDFA_H__
#define __KEYWORDDFA_H__

#include "dfa.h"

class KeywordDfa : public Dfa {
    private:
        void initDfa();
    public:
        KeywordDfa();
};

#endif
