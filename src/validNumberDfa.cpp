#include "states.h"
#include "validNumberDfa.h"

int isNonZeroDigit(char c, int current_state) {
    if('1' <= c && c <= '9') {
        return DS_ACCEPT;
    } else if(c == '0') {
        return DS_ZERO;
    }

    return DS_ERROR;
}

int isDigitOrUnderscore(char c, int current_state) {
    if('0' <= c && c <= '9') {
        return DS_ACCEPT;
    } else if(c == '_') {
        return DS_UNDERSCORE;
    }

    return DS_ERROR;
}

void ValidNumberDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isNonZeroDigit);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &isDigitOrUnderscore);
    dfa[DS_ZERO] = std::make_pair(DS_ACCEPT, &error);
    dfa[DS_UNDERSCORE] = std::make_pair(DS_RUNNING, &isDigitOrUnderscore);
}

ValidNumberDfa::ValidNumberDfa() : Dfa() {
    initDfa();
}
