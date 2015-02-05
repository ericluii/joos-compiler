#ifndef __SINGLECOMMENT_H__
#define __SINGLECOMMENT_H__

#include "dfa.h"
#include <string>

class SingleCommentDfa : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        SingleCommentDfa();
        
        static TOKEN_TYPE endTokenType;
};

#endif
