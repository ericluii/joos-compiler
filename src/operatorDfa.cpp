#include "operatorDfa.h"
#include "states.h"

std::string OperatorDfa::operators = "=><!&|+-*/%";

int isFirstCharOperator(char c, int current_state) {
    if(OperatorDfa::operators.find(c) != std::string::npos) {
        if(c == '=' || c == '>' || c == '<' || c == '!') {
            return DS_CHECKEQUAL;
        } else if(c == '&') {
            return DS_CHECKAMPERSAND;
        } else if(c == '|') {
            return DS_CHECKOR;
        } else {
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int isEqualSign(char c, int current_state) {
    if(c == '=') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int isAmpersandSign(char c, int current_state) {
    if(c == '&') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int isOrSign(char c, int current_state) {
    if(c == '|') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void OperatorDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isFirstCharOperator);
    dfa[DS_CHECKEQUAL] = std::make_pair(DS_ACCEPT, &isEqualSign);
    dfa[DS_CHECKAMPERSAND] = std::make_pair(DS_ACCEPT, &isAmpersandSign);
    dfa[DS_CHECKOR] = std::make_pair(DS_ACCEPT, &isOrSign);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &error);
}

OperatorDfa::OperatorDfa() : Dfa() {
    initDfa();
}
