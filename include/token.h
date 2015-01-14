#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TOKEN_TYPE {
    TT_NUM,
    TT_WORD,
    TT_KEYWORD,
    TT_OPERATOR,
    TT_SEPARATOR,
    TT_EOF,
    TT_INVALID,
    TT_ID,

    // Char literal token type
    TT_CHARACTER,
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

    // Keyword States
    TT_DO,
    TT_IF,
    TT_FOR,
    TT_INT,
    TT_NEW,
    TT_TRY,
    TT_BYTE,
    TT_CASE,
    TT_CHAR,
    TT_ELSE,
    TT_ENUM,
    TT_GOTO,
    TT_LONG,
    TT_THIS,
    TT_VOID,
    TT_BREAK,
    TT_CATCH,
    TT_CLASS,
    TT_CONST,
    TT_FINAL,
    TT_FLOAT,
    TT_SHORT,
    TT_SUPER,
    TT_THROW,
    TT_WHILE,
    TT_ASSERT,
    TT_DOUBLE,
    TT_IMPORT,
    TT_NATIVE,
    TT_PUBLIC,
    TT_RETURN,
    TT_STATIC,
    TT_SWITCH,
    TT_THROWS,
    TT_BOOLEAN,
    TT_DEFAULT,
    TT_EXTENDS,
    TT_FINALLY,
    TT_PACKAGE,
    TT_PRIVATE,
    TT_ABSTRACT,
    TT_CONTINUE,
    TT_STRICTFP,
    TT_VOLATILE,
    TT_INTERFACE,
    TT_PROTECTED,
    TT_TRANSIENT,
    TT_IMPLEMENTS,
    TT_INSTANCEOF,
    TT_SYNCHRONIZED,
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
