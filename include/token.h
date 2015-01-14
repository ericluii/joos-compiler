#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TOKEN_TYPE {
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
		
		Token(TOKEN_TYPE ptype, std::string pstring, std::pair <unsigned int, unsigned int> plocation);
};

#endif
