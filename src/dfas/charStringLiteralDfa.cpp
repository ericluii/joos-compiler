#include "charStringLiteralDfa.h"
#include "states.h"
#include <cstdio>
#include <cstdlib>

std::string CharStringLiteralDfa::escapeSequence = "btnfr\"\'\\";
unsigned int CharStringLiteralDfa::octalCounter = 0;
unsigned int CharStringLiteralDfa::octalLimit = 0;
std::string CharStringLiteralDfa::replacement = "";

void CharStringLiteralDfa::generateHexForOctal(){
    char result[10];
    long value = std::strtol(replacement.c_str(), NULL, 8);
    std::sprintf(result, "\\u00%lX", value);
    replacement = "";
    replacement.append(result);
}

void CharStringLiteralDfa::performEscapeSequenceReplacement()
{
    if(replacement == "b"){
        replacement = '\b';
    }
    else if(replacement == "t"){
        replacement = 't';
    }
    else if(replacement == "n"){
        replacement = '\n';
    }
    else if(replacement == "f"){
        replacement = '\f';
    }
    else if(replacement == "r"){
        replacement = '\r';
    }
    else if(replacement == "\""){
        replacement = '\"';
    }
    else if(replacement == "\'"){
        replacement = '\'';
    }
    else if(replacement == "\\"){
        replacement = '\\';
    }
}

int isSingleOrDoubleQuotes(char c, int current_state) {
    // identifies whether it's a character or string literal
    CharStringLiteralDfa::replacement = "";
    if(c == '\'') {
        return DS_SINGLEQUOTE;
    } else if(c == '"') {
        return DS_DOUBLEQUOTE;
    }

    return DS_ERROR;
}

int genericCharCheck(char c, int current_state) {
    // cases:
    // - Current state is DS_SINGLEQUOTE:
    //      * single quote before seeing any characters -> go to DS_ERROR
    //      * a \ is seen indicating escape sequence -> go to DS_ESCAPESINGLE
    //      * everything else -> go to DS_ENCLOSESINGLE
    // - Current state is DS_DOUBLEQUOTE:
    //      * double quote signals end of string literal -> go to DS_ACCEPTSTRING
    //      * a \ is seen indicating escape sequence -> go to DS_ESCAPEDOUBLE
    //      * everything else -> loop around DS_DOUBLEQUOTE
    if(c == '\\') { 
        if(current_state == DS_SINGLEQUOTE) {
            return DS_ESCAPESINGLE;
        } else {
            return DS_ESCAPEDOUBLE;
        }
    }
   
    if(c == '\n' || c == '\r') {
        // a line terminator is seen -> error out
        return DS_ERROR;
    }

    if(current_state == DS_SINGLEQUOTE) {
        if(c == '\'') {
            return DS_ERROR;
        } else {
            return DS_ENCLOSESINGLE;
        }
    } else {
        if(c == '"') {
            return DS_ACCEPTSTRING;
        } else {
            return DS_DOUBLEQUOTE;
        }
    }
}

int isSingleQuoteClosing(char c, int current_state) {
    // the next character should be a single quote to close the char literal
    if(c == '\'') {
        return DS_ACCEPTCHAR;
    }

    return DS_ERROR;
}


int genericEscapeCheck(char c, int current_state) {
    // checks if the next character is legitimate for an escape sequence
    // cases:
    // - Char literal:
    //      * it is an escape sequence -> go to DS_ENCLOSESINGLE
    //      * it is an octal sequence -> go to DS_OCTALSINGLE
    // - String literal:
    //      * it is an escape sequence -> go to DS_DOUBLEQUOTE
    //      * it is an octal sequence -> go to DS_OCTALDOUBLE
    CharStringLiteralDfa::replacement += c;
    if(CharStringLiteralDfa::escapeSequence.find(c) != std::string::npos) {
        CharStringLiteralDfa::performEscapeSequenceReplacement();
        if(current_state == DS_ESCAPESINGLE) {
            return DS_ENCLOSESINGLE;
        } else {
            return DS_DOUBLEQUOTE;
        }
    } else if('0' <= c && c <= '7') {
        CharStringLiteralDfa::octalCounter+= 1;
        CharStringLiteralDfa::octalLimit = 2;
        if('0' <= c && c <= '3') {
            CharStringLiteralDfa::octalLimit+= 1;
        }

        if(current_state == DS_ESCAPESINGLE) {
            return DS_OCTALSINGLE;
        } else {
            return DS_OCTALDOUBLE;
        }
    }

    return DS_ERROR;
}


int genericOctalCheck(char c, int current_state) {
    // checks if it's a legitimate octal escape for a char literal
    // cases:
    // - Char literal:
    //      * the character is a part of an octal escape -> loop back to DS_OCTALSINGLE
    //      * the character is a part of an octal escape and has reached the maximum number of
    //      characters allowed in an octal escape, which is 3 -> go to DS_ENCLOSESINGLE
    //      * the character is a single quote -> go to DS_ACCEPTCHAR
    //      * all other cases -> go to DS_ERROR
    // - String literal:
    //      * the character is a part of an octal escape -> loop back to DS_OCTALDOUBLE
    //      * the character is a part of an octal escape and has reached the maximum number of
    //      characters allowed in an octal escape, which is 3 -> go to DS_DOUBLEQUOTE
    //      * the character is a double quote -> go to DS_ACCEPTSTRING
    //      * all other cases -> go to DS_DOUBLEQUOTE

    if('0' <= c && c <= '7') {
        CharStringLiteralDfa::octalCounter+= 1;
        if(CharStringLiteralDfa::octalCounter == CharStringLiteralDfa::octalLimit) {
            CharStringLiteralDfa::octalCounter = 0;
            CharStringLiteralDfa::octalLimit = 0;
            CharStringLiteralDfa::generateHexForOctal();
            if(current_state == DS_OCTALSINGLE) {
                return DS_ENCLOSESINGLE;
            } else {
                return DS_DOUBLEQUOTE;
            }
        }

        if(current_state == DS_OCTALSINGLE) {
            return DS_OCTALSINGLE;
        } else {
            return DS_OCTALDOUBLE;
        }
    } else if(c == '\'' && current_state == DS_OCTALSINGLE) {
        CharStringLiteralDfa::octalCounter = 0;
        CharStringLiteralDfa::octalLimit = 0;
        CharStringLiteralDfa::generateHexForOctal();
        return DS_ACCEPTCHAR;
    } else if(c == '"' && current_state == DS_OCTALDOUBLE) {
        CharStringLiteralDfa::octalCounter = 0;
        CharStringLiteralDfa::octalLimit = 0;
        CharStringLiteralDfa::generateHexForOctal();
        return DS_ACCEPTSTRING;
    }

    CharStringLiteralDfa::octalCounter = 0;
    CharStringLiteralDfa::octalLimit = 0;
    if(current_state == DS_OCTALSINGLE) {
        return DS_ERROR;
    } else {
        return DS_DOUBLEQUOTE;
    }
}

void CharStringLiteralDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &dfa_error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &isSingleOrDoubleQuotes);
    dfa[DS_SINGLEQUOTE] = std::make_pair(DS_RUNNING, &genericCharCheck);
    dfa[DS_DOUBLEQUOTE] = std::make_pair(DS_RUNNING, &genericCharCheck);
    dfa[DS_ENCLOSESINGLE] = std::make_pair(DS_RUNNING, &isSingleQuoteClosing);
    dfa[DS_ESCAPESINGLE] = std::make_pair(DS_RUNNING, &genericEscapeCheck);
    dfa[DS_ESCAPEDOUBLE] = std::make_pair(DS_RUNNING, &genericEscapeCheck);
    dfa[DS_OCTALSINGLE] = std::make_pair(DS_RUNNING, &genericOctalCheck);
    dfa[DS_OCTALDOUBLE] = std::make_pair(DS_RUNNING, &genericOctalCheck);
    dfa[DS_ACCEPTCHAR] = std::make_pair(DS_ACCEPT, &dfa_error);
    dfa[DS_ACCEPTSTRING] = std::make_pair(DS_ACCEPT, &dfa_error);
}

TOKEN_TYPE CharStringLiteralDfa::getTokenType() {
    if(current_state == DS_ACCEPTCHAR) {
        return TT_CHARACTER;
    } else if(current_state == DS_ACCEPTSTRING) {
        return TT_STRING;
    }

    return TT_INVALID;
}


CharStringLiteralDfa::CharStringLiteralDfa() : Dfa() {
    initDfa();
}
