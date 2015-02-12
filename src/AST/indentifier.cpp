#include "identifier.h"

Identifier::Identifier(Token *token){
    this->token = token;
    this->id = token->getString();
}

std::string Identifier::GetId(){
    return id;
}

