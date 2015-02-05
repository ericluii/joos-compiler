#ifndef __ERROR_H__
#define __ERROR_H__

#include <vector>
#include "token.h"

enum E_TYPE {
    E_SCANNER,
    E_PARSER,
    E_WEEDER
};

class Error {
    private:
        E_TYPE type;
        Token* token;
        std::string message;

        static std::vector<Error> all_errors;
    public:
        Error(E_TYPE type, Token* token, std::string message);

        static void print();
        static unsigned int count();
        // WARNING: DO NOT USE ANYWHERE EXCEPT FOR TESTING.
        static void resetErrors();
};

#endif
