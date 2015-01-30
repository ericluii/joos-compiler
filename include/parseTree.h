#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "token.h"
#include <map>
#include <vector>

class ParseTree final
{
    public:
        int rule;
        Token* token;
        std::vector<ParseTree*> children;
        ParseTree(Token* token, int ruleNum);
        ~ParseTree()
        {
            for (unsigned int i = 0; i < children.size(); i++) {
                delete children[i];
            }
        }

        void addChild(ParseTree* child);
};

#endif
