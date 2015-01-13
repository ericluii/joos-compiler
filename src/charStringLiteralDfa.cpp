#include "charStringLiteralDfa.h"
#include "states.h"

int charStringLiteralError(char c) {
    return DS_ERROR;
}

int isSingleOrDoubleQuotes(char c) {
    // identifies whether it's a character or string literal
    if(c == '\'') {
        return DS_SINGLEQUOTE;
    } else if(c == '"') {
        return DS_DOUBLEQUOTE;
    }

    return DS_ERROR;
}

int singleQuoteCheckChar(char c) {
    // the choices are:
    // single quote before seeing any characters -> go to DS_ERROR
    // a \ is seen indicating escape sequence -> go to DS_ESCAPESINGLE
    // everything else -> go to DS_ENCLOSESINGLE
    if(c == '\'') {
        return DS_ERROR;
    } else if(c == '\\') {
        return DS_ESCAPESINGLE;
    } else {
        return DS_ENCLOSESINGLE;
    }
}

int isSingleQuoteClosing(char c) {
    // the next character should be a single quote to close the char literal
    if(c == '\'') {
        return DS_ACCEPT;
    }

    return DS_ERROR;
}

int doubleQuoteCheckChar(char c) {
    // the choices are:
    // double quote signals end of string literal -> go to DS_ACCEPT
    // a \ is seen indicating escape sequence -> go to DS_ESCAPEDOUBLE
    // everything else -> loop around DS_DOUBLEQUOTE
    if(c == '"') {
        return DS_ACCEPT;
    } else if(c == '\\') {
        return DS_ESCAPEDOUBLE;
    } else {
        return DS_DOUBLEQUOTE;
    }
}

int singleQuoteEscape(char c) {
    // checks if the next character is legitimate for an escape sequence
    // transitions based on the fact that the current word is a char literal
    // cases:
    // it is an escape sequence -> go to DS_ENCLOSESINGLE
    // it is an octal sequence -> go to DS_OCTALSINGLE
    if(escapeSequence.find(c) != std::string::npos) {
        return DS_ENCLOSESINGLE;
    } else if(c == '0' || c == '1') {
        return DS_OCTALSINGLE;
    }

    return DS_ERROR;
}

int doubleQuoteEscape(char c) {
    // checks if the next character is legitimate for an escape sequence
    // transitions based on the fact that the current word is a string literal
    // cases:
    // it is an escape sequence -> go to DS_DOUBLEQUOTE
    // it is an octal sequence -> go to DS_OCTALDOUBLE
    if(escapeSequence.find(c) != std::string::npos) {
        return DS_DOUBLEQUOTE;
    } else if(c == '0' || c == '1') {
        octalCounter+= 1;
        return DS_OCTALDOUBLE;
    }

    return DS_ERROR;
}

int singleQuoteOctalCheck(char c) {
    // checks if it's a legitimate octal escape for a char literal
    // cases:
    // - the character is a part of an octal escape -> loop back to DS_OCTALSINGLE
    // - the character is a part of an octal escape and has reached the maximum number of
    // characters allowed in an octal escape, which is 3 -> go to DS_ENCLOSESINGLE
    // - the character is a single quote -> go to DS_ACCEPT
    // - all other cases -> go to DS_ERROR
    if('0' <= c && c <= '7') {
        octalCounter+= 1;
        if(octalCounter == octalLimit) {
            octalCounter = 0;
            return DS_ENCLOSESINGLE;
        }
        return DS_OCTALSINGLE;
    } else if(c == '\'') {
        octalCounter = 0;
        return DS_ACCEPT;
    }

    octalCounter = 0;
    return DS_ERROR;
}

int doubleQuoteOctalCheck(char c) {
    // checks if it's a legitimate octal escape for a string literal
    // cases:
    // the character is a part of an octal escape -> loop back to DS_OCTALDOUBLE
    // the character is a part of an octal escape and has reached the maximum number of
    // characters allowed in an octal escape, which is 3 -> go to DS_DOUBLEQUOTE
    // the character is a double quote -> go to DS_ACCEPT
    // all other cases -> go to DS_ERROR
    if('0' <= c && c <= '7') {
        octalCounter+= 1;
        if(octalCounter == octalLimit) {
            octalCounter = 0;
            return DS_DOUBLEQUOTE;
        }
        return DS_OCTALDOUBLE;
    } else if(c == '"') {
        octalCounter = 0;
        return DS_ACCEPT;
    }

    octalCounter = 0;
    return DS_DOUBLEQUOTE;
}

void CharStringLiteralDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &charStringLiteralError);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isSingleOrDoubleQuotes);
    dfa[DS_SINGLEQUOTE] = std::make_pair(DS_RUNNING, &singleQuoteCheckChar);
    dfa[DS_ENCLOSESINGLE] = std::make_pair(DS_RUNNING, &isSingleQuoteClosing);
    dfa[DS_DOUBLEQUOTE] = std::make_pair(DS_RUNNING, &doubleQuoteCheckChar);
    dfa[DS_ESCAPESINGLE] = std::make_pair(DS_RUNNING, &singleQuoteEscape);
    dfa[DS_ESCAPEDOUBLE] = std::make_pair(DS_RUNNING, &doubleQuoteEscape);
    dfa[DS_OCTALSINGLE] = std::make_pair(DS_RUNNING, &singleQuoteOctalCheck);
    dfa[DS_OCTALDOUBLE] = std::make_pair(DS_RUNNING, &doubleQuoteOctalCheck);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &charStringLiteralError);
}

CharStringLiteralDfa::CharStringLiteralDfa() : Dfa() {
    initDfa();
}
