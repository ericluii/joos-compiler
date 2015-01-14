#include "token.h"

TOKEN_TYPE Token::getType()
{
    return this->type;
}

std::string Token::getString()
{
    return this->string;
}

std::pair <unsigned int, unsigned int> Token::getLocation()
{
    return this->location;
}

Token::Token(TOKEN_TYPE ptype, std::string pstring, std::pair <unsigned int, unsigned int> plocation)
{
	this->type = ptype;
	this->string = pstring;
	this->location = plocation;
}
