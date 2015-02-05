#include "error.h"
#include <iostream>

std::vector<Error> Error::all_errors;

Error::Error(E_TYPE type, Token* token, std::string message)
{
    this->type = type;
    this->token = token;
    this->message = message;

    this->all_errors.push_back(*this);
}

// WARNING: DO NOT USE ANYWHERE EXCEPT FOR TESTING.
void Error::resetErrors() {
    all_errors.clear();
}

unsigned int Error::count() {
    return all_errors.size();
}

void Error::print()
{
    for (int i = 0; i < all_errors.size(); i++) {
        std::cerr << all_errors[i].token->getFile() << ":"
                  << all_errors[i].token->getLocation().first << ":"
                  << all_errors[i].token->getLocation().second << ": error: "
                  << all_errors[i].message << std::endl;
    }
    std::cerr << all_errors.size() << " Error(s) generated." << std::endl;
}
