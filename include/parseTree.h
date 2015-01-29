#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "token.h"
#include <map>
#include <vector>

class ParseTree final
{
    public:
        int rule;
        std::vector<Token *> tokens;
        std::vector<ParseTree *> children;
        ~ParseTree()
        {
            for (int i = 0; i < children.size(); i++) {
                delete children[i];
            }
        }
};

ParseTree* buildParseTree(std::map<std::string, std::vector<Token*> *> &tokens);

#endif
