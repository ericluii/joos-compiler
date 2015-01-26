#include "weeder.h"
#include <cassert>

Weeder::Weeder(std::map<std::string, std::string> &st) : symbolTable(st)
{
    didGenSymbolTable = false;

    // Add 'weeds' that need to be veried to this vector
    weeds.push_back(new DemoWeed());    
    weeds.push_back(new DemoWeed2());    
}

int Weeder::buildSymbolTable(ParseTree* node)
{
    return 0;
}

int Weeder::weedParseTree(ParseTree* node)
{
    int errors = 0;

    // Try to match against all weeds
    for (int i = 0; i < weeds.size(); i++) {
        if (weeds[i]->rule == node->rule) {
            errors += weeds[i]->check(node);
        }
    }

    for (int j = 0; j < node->children.size(); j++) {
        errors += weedParseTree(node->children[j]);
    }

    return errors;
}
