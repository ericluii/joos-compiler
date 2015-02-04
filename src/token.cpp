#include "token.h"

TOKEN_TYPE Token::getType()
{
    return this->type;
}

std::string Token::getString()
{
    return this->string;
}

std::string Token::getFile()
{
    return this->file;
}

std::pair <unsigned int, unsigned int> Token::getLocation()
{
    return this->location;
}

Token::Token(TOKEN_TYPE ptype, std::string pstring, std::pair <unsigned int, unsigned int> plocation, std::string file)
{
    this->type = ptype;
    this->string = pstring;
    this->location = plocation;
    this->file = file;
}
