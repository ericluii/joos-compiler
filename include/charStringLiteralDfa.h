#ifndef __CHARSTRINGLITERALDFA_H__
#define __CHARSTRINGLITERALDFA_H__

#include "dfa.h"
#include <string>

class CharStringLiteralDfa final : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        CharStringLiteralDfa();
        
        static std::string escapeSequence;
        // limit and counter pertaining to octal sequences
        static unsigned int octalCounter;
        static unsigned int octalLimit;
};

#endif
