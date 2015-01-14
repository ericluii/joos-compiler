#include "dfa.h"
#include "states.h"
#include <assert.h>

int error(char c, int current_state) {
    return DS_ERROR;
}

Dfa::Dfa()
{
    current_state = DS_START;
}

TOKEN_TYPE Dfa::getTokenType()
{
    throw "Inheriting Dfa did not Implement.";
}

std::pair<int, int> Dfa::transition(char c)
{
    assert(dfa.find(current_state) != dfa.end());
    current_state = dfa[current_state].second(c, current_state);

    return getStatus();
}

std::pair<int, int> Dfa::getStatus()
{
    assert(dfa.find(current_state) != dfa.end());

    return std::make_pair(dfa[current_state].first, getTokenType());
}

void Dfa::resetDfa()
{
    current_state = DS_START;
}
