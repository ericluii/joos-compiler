#ifndef __SINGLECOMMENT_H__
#define __SINGLECOMMENT_H__

#include "dfa.h"
#include <string>

class SingleCommentDfa final : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        SingleCommentDfa();
        
        static TOKEN_TYPE endTokenType;
};

#endif
