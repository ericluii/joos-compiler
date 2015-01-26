#include "whitespaceAndControlDfa.h"
#include "states.h"

TOKEN_TYPE WhitespaceAndControlDfa::endTokenType = TT_INVALID;

int checkCharacter(char c, int current_state)
{
    //currently matches EOF as well
    //we can change this if we ever want to create an EOF token
    if(c <= 32 || c == 127) {
        WhitespaceAndControlDfa::endTokenType = TT_WHITESPACE;
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void WhitespaceAndControlDfa::initDfa()
{
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &checkCharacter);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &checkCharacter);
}

TOKEN_TYPE WhitespaceAndControlDfa::getTokenType()
{
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return endTokenType;
}

WhitespaceAndControlDfa::WhitespaceAndControlDfa() : Dfa()
{
    initDfa();
}
