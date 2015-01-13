#include "states.h"
#include "validNumberDfa.h"

int isNonZeroDigit(char c, int current_state) {
    if('1' <= c && c <= '9') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int isDigit(char c, int current_state) {
    if('0' <= c && c <= '9') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void ValidNumberDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isNonZeroDigit);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &isDigit);
}

ValidNumberDfa::ValidNumberDfa() : Dfa() {
    initDfa();
}
