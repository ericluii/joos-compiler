#ifndef __SEPARATOR_H__
#define __SEPARATOR_H__

#include "dfa.h"
#include <string>

class SeparatorDfa : public Dfa {
    private:
        
        void initDfa();
        TOKEN_TYPE getTokenType();
    public:
        SeparatorDfa();
        
        static std::string separators;
        static TOKEN_TYPE endTokenType;
};

#endif
