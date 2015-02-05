#ifndef __MULTICOMMENT_H__
#define __MULTICOMMENT_H__

#include "dfa.h"
#include <string>

class MultiCommentDfa : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        MultiCommentDfa();
        
        static TOKEN_TYPE endTokenType;
};

#endif
