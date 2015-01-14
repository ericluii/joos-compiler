#include "separatorDfa.h"
#include "states.h"

std::string SeparatorDfa::separators = "(){}[];,.@:";
TOKEN_TYPE SeparatorDfa::endTokenType = TT_INVALID;

int checkSeparator(char c, int current_state) {
    if(SeparatorDfa::separators.find(c) != std::string::npos) {
        if(c == '.') {
            SeparatorDfa::endTokenType = TT_DOT;
            return DS_DOTFIRST;
        } else if(c == ':') {
            return DS_COLON;
        } else {
            if(c == '(') {
                SeparatorDfa::endTokenType = TT_LPARAN;
            } else if(c == ')') {
                SeparatorDfa::endTokenType = TT_RPARAN;
            } else if(c == '{') {
                SeparatorDfa::endTokenType = TT_LBRACK;
            } else if(c == '}') {
                SeparatorDfa::endTokenType = TT_RBRACK;
            } else if(c == '[') {
                SeparatorDfa::endTokenType = TT_LSQBRACK;
            } else if(c == ']') {
                SeparatorDfa::endTokenType = TT_RSQBRACK;
            } else if(c == ';') {
                SeparatorDfa::endTokenType = TT_SEMICOLON;
            } else {
                SeparatorDfa::endTokenType = TT_COMMA;
            }

            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int dotSeparator(char c, int current_state) {
    if(c == '.') {
        if(current_state == DS_DOTFIRST) {
            return DS_DOTSECOND;
        } else {
            SeparatorDfa::endTokenType = TT_TRIPLEDOT;
            return DS_ACCEPT;
        }
    }

    return DS_ERROR;
}

int colonSeparator(char c, int current_state) {
    if(c == ':') {
        SeparatorDfa::endTokenType = TT_COLONCOLON;
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

void SeparatorDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &checkSeparator);
    dfa[DS_DOTFIRST] = std::make_pair(DS_ACCEPT, &dotSeparator);
    dfa[DS_DOTSECOND] = std::make_pair(DS_RUNNING, &dotSeparator);
    dfa[DS_COLON] = std::make_pair(DS_RUNNING, &colonSeparator);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &error);
}

TOKEN_TYPE SeparatorDfa::getTokenType() {
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return endTokenType;
}

SeparatorDfa::SeparatorDfa() : Dfa() {
    initDfa();
}
