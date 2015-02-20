#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

#include "token.h"
#include "ast.h"

class Identifier : public Ast
{
    // Rule: IDENTIFIER
    private:
        Token *token;
    public:
        Identifier(Token *token) : token(token) {}
        std::string getIdAsString() { return token->getString(); }
};

#endif
