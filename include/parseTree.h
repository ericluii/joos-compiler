#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "token.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>

class ParseTree final
{
    public:
        int rule;
        Token* token;
        std::string treeLexeme;
        std::vector<ParseTree*> children;
        ParseTree(Token* token, int ruleNum, std::string treeLexeme);
        ~ParseTree()
        {
            for (unsigned int i = 0; i < children.size(); i++) {
                delete children[i];
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const ParseTree & parseTree);

        void addChild(ParseTree* child);
        void print(int depth = 0);
};

#endif
