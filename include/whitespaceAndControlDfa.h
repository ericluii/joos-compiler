#ifndef __WHITESPACE_H__
#define __WHITESPACE_H__

#include "dfa.h"
#include <string>

class WhitespaceAndControlDfa final : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        WhitespaceAndControlDfa();
        
        static TOKEN_TYPE endTokenType;
};

#endif