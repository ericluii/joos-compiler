#ifndef __IDENTIFIER_H__
#define __IDENTIFIER_H__

#include <string>
#include "../token.h"

class Identifier
{
        Token *token;
        std::string id;
    public:
        
        Identifier(Token *token);
        
        std::string GetId();
};

#endif
