#ifndef __DFA_H__
#define __DFA_H__

#include <map>

typedef int (*TransitionFunction)(char);

class Dfa {
    private:
        bool is_initialized;
    protected:
        std::map<int, std::pair<int, TransitionFunction> > dfa;
        int start_state;
        int current_state;

        Dfa();
        // Method is responsible for setting the start state
        // and the dfa representation
        virtual void initDfa() = 0;
    public:
        void initialize();
        int transition(char c);
        int onAcceptingState();
        void resetDfa();
};

#endif
