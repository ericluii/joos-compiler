#include "identifierDfa.h"
#include "states.h"

int isJavaLetterOrDigit(char c, int current_state) {
    // checks if character is a java letter AND digit if and only if current
    // state is not DS_START, which means it must be DS_ACCEPT
    if(current_state == DS_START) {
        if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || c == '$') {
            return DS_ACCEPT;
        }
    } else {
        if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') 
           || c == '_' || c == '$') {
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

void IdentifierDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isJavaLetterOrDigit);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &isJavaLetterOrDigit);
}

TOKEN_TYPE IdentifierDfa::getTokenType() {
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return TT_ID;
}

IdentifierDfa::IdentifierDfa() : Dfa() {
    initDfa();
}
