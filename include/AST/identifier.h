#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

#include <string>
#include "../token.h"
#include "ast.h"

class Identifier : public Ast
{
        Token *token;
    public:
        Identifier(Token *token) : token(token) {}
        std::string GetId() { return token->getString(); }
};

#endif
