#ifndef __DFA_H__
#define __DFA_H__

#include "token.h"
#include <map>

typedef int (*TransitionFunction)(char, int);

int error(char c, int current_state);

class Dfa {
    protected:
        std::map<int, std::pair<int, TransitionFunction> > dfa;
        int current_state;

        Dfa();
        // Method is responsible for setting the start state
        // and the dfa representation
        virtual void initDfa() = 0;
        virtual TOKEN_TYPE getTokenType();
    public:
        void initialize();
        std::pair<int, int> transition(char c);
        std::pair<int, int> getStatus();
        void resetDfa();
};

#endif
