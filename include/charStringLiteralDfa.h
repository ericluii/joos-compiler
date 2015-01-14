#ifndef __CHARSTRINGLITERALDFA_H__
#define __CHARSTRINGLITERALDFA_H__

#include "dfa.h"
#include <string>

#define OCTAL_LIMIT 3

class CharStringLiteralDfa final : public Dfa {
    private:
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        CharStringLiteralDfa();
        
        static std::string escapeSequence;
        // limit and counter pertaining to octal sequences
        static unsigned int octalCounter;
};

#endif
