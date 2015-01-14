#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TOKEN_TYPE {
    // Signifies invalid token
    TT_INVALID,

    // Identifier token type
    TT_ID,

    // Char literal token type
    TT_CHAR,
    // String literal token type
    TT_STRING,

    // Separator token types
    TT_LPARAN,
    TT_RPARAN,
    TT_LBRACK,
    TT_RBRACK,
    TT_LSQBRACK,
    TT_RSQBRACK,
    TT_SEMICOLON,
    TT_COMMA,
    TT_DOT,
    TT_TRIPLEDOT,
    TT_AT,
    TT_COLONCOLON,

    // Operator token types
    TT_ASSIGNMENT,
    TT_MATHOP,
    TT_COMPARISONOP,
    TT_NOT,
    TT_BINBOOLOPEAGER,
    TT_BINBOOLOPLAZY,

    TT_WORD,
    TT_KEYWORD,
    TT_OPERATOR,
    TT_SEPARATOR,
    TT_EOF,
};

class Token {
    private:
        TOKEN_TYPE type;
        std::string string;
        std::pair <unsigned int, unsigned int> location;
    public:
        TOKEN_TYPE getType();
        std::string getString();
        std::pair <unsigned int, unsigned int> getLocation();
};

#endif
