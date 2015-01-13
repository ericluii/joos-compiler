#include "identifierDfa.h"
#include "states.h"

int identifierError(char c) {
    return DS_ERROR;
}

int isJavaLetter(char c) {
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || c == '$') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int isJavaLetterOrDigit(char c) {
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') 
            || c == '_' || c == '$') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void IdentifierDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &identifierError);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isJavaLetter);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &isJavaLetterOrDigit);
}

IdentifierDfa::IdentifierDfa() {
    initDfa();
}
