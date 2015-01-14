#include "separatorDfa.h"
#include "states.h"

std::string SeparatorDfa::separators = "(){}[];,.@:";

int checkSeparator(char c, int current_state) {
    if(SeparatorDfa::separators.find(c) != std::string::npos) {
        if(c == '.') {
            return DS_DOTSECOND;
        } else if(c == ':') {
            return DS_COLON;
        } else {
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int dotSeparator(char c, int current_state) {
    if(c == '.') {
        if(current_state == DS_DOTSECOND) {
            return DS_DOTTHIRD;
        } else {
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int colonSeparator(char c, int current_state) {
    if(c == ':') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void SeparatorDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &checkSeparator);
    dfa[DS_DOTSECOND] = std::make_pair(DS_RUNNING, &dotSeparator);
    dfa[DS_DOTTHIRD] = std::make_pair(DS_RUNNING, &dotSeparator);
    dfa[DS_COLON] = std::make_pair(DS_RUNNING, &colonSeparator);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &error);
}

SeparatorDfa::SeparatorDfa() : Dfa() {
    initDfa();
}
