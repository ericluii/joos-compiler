#include "dfa.h"
#include <assert.h>

Dfa::Dfa()
{
    this->dfa = std::map<int, std::pair<int, TransitionFunction> >();
    // TODO: Change to error state macro
    this->start_state = -1;
}

void Dfa::initialize()
{
    current_state = start_state;

    // TODO: Change to error state macro
    assert(start_state != -1);

    is_initialized = true;
}

int Dfa::transition(char c)
{
    assert(this->is_initialized);
    assert(dfa.find(current_state) != dfa.end());

    current_state = dfa[current_state].second(c);

    return 0;
}

int Dfa::onAcceptingState()
{
    assert(dfa.find(current_state) != dfa.end());

    // TODO: Change to accepting state macro
    return dfa[current_state].first != 0;
}

void Dfa::resetDfa()
{
    current_state = start_state;
}
