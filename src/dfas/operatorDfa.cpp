#include "operatorDfa.h"
#include "states.h"

std::string OperatorDfa::operators = "=><!&|+-*/%&";
TOKEN_TYPE OperatorDfa::endTokenType = TT_INVALID;
char OperatorDfa::previousChar = '\0';

int isFirstCharOperator(char c, int current_state) {
    if(OperatorDfa::operators.find(c) != std::string::npos) {
        if(c == '=' || c == '>' || c == '<' || c == '!') {
            if(c == '=') {
                OperatorDfa::endTokenType = TT_ASSIGNMENT;
            } else if(c == '!') {
                OperatorDfa::endTokenType = TT_NOT;
            } else {
                OperatorDfa::endTokenType = TT_COMPARISONOP;
            }
            return DS_CHECKEQUAL;
        } else if(c == '&' || c == '|') {
            OperatorDfa::endTokenType = TT_BINBOOLOPEAGER;
            if(c == '&' || c == '|') {
                OperatorDfa::previousChar = c;
                return DS_CHECKLAZYOP;
            } else {
                return DS_ACCEPT;
            }
        } else {
            OperatorDfa::endTokenType = TT_MATHOP;
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int isLazyBooleanOp(char c, int current_state) {
    // Checks whether or not the boolean operator is lazy
    if((c == '&' && OperatorDfa::previousChar == '&') || (c == '|' && OperatorDfa::previousChar == '|')) {
        OperatorDfa::endTokenType = TT_BINBOOLOPLAZY;
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int isEqualSign(char c, int current_state) {
    if(c == '=') {
        OperatorDfa::endTokenType = TT_COMPARISONOP;
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void OperatorDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &dfa_error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isFirstCharOperator);
    dfa[DS_CHECKEQUAL] = std::make_pair(DS_ACCEPT, &isEqualSign);
    dfa[DS_CHECKLAZYOP] = std::make_pair(DS_ACCEPT, &isLazyBooleanOp);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &dfa_error);
}

TOKEN_TYPE OperatorDfa::getTokenType() {
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return endTokenType;
}

OperatorDfa::OperatorDfa() : Dfa() {
    initDfa();
}
