#include "bannedOperatorDfa.h"
#include "states.h"

TOKEN_TYPE BannedOperatorDfa::endTokenType = TT_INVALID;

int isPlusOrMinus(char c, int current_state) {
    if(current_state == DS_START) {
        if(c == '+') {
            return DS_PLUS;
        } else if(c == '-') {
            return DS_MINUS;
        }
    } else if(current_state == DS_PLUS) {
        if(c == '+') {
            BannedOperatorDfa::endTokenType = TT_PLUSPLUS;
            return DS_ABORT;
        }
    } else if(current_state == DS_MINUS) {
        if(c == '-') {
            BannedOperatorDfa::endTokenType = TT_MINUSMINUS;
            return DS_ABORT;
        }
    }

    return DS_ERROR;
}

void BannedOperatorDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &dfa_error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isPlusOrMinus);
    dfa[DS_PLUS] = std::make_pair(DS_RUNNING, &isPlusOrMinus);
    dfa[DS_MINUS] = std::make_pair(DS_RUNNING, &isPlusOrMinus);
    dfa[DS_ABORT] = std::make_pair(DS_ABORT, &dfa_error);
}

TOKEN_TYPE BannedOperatorDfa::getTokenType() {
    if(dfa[current_state].first != DS_ABORT) {
        return TT_INVALID;
    }

    return endTokenType;
}

BannedOperatorDfa::BannedOperatorDfa() {
    initDfa();
}
