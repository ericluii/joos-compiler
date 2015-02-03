#include "parseTree.h"
#include "ruleNumbers.h"
#include "parserRules.h"
#include <string>

ParseTree::ParseTree(Token* token, int ruleNum, std::string treeLexeme) : rule(ruleNum), token(token), treeLexeme(treeLexeme) {}

void ParseTree::addChild(ParseTree* child) {
    children.push_back(child);
}

void ParseTree::print(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << ' ';
    }

    std::cout << treeLexeme << ":" << rule;
    if (token != NULL) {
        std::cout << ":" << token->getString();
    }
    std::cout << std::endl;

    for (unsigned int i = 0; i < children.size(); i++) {
        children[i]->print(depth + 1);
    }
}

std::ostream& operator<<(std::ostream& os, const ParseTree& parseTree) {
    os << parseTree.treeLexeme;
    if(parseTree.token == NULL) {
        os << "'s children:";
        for(unsigned int i = 0; i < parseTree.children.size(); i++) {
            if(i != 0) {
                os << ", ";
            }
            os << (parseTree.children[i])->treeLexeme;
        }

        os << '\n';
        for(unsigned int i = 0; i < parseTree.children.size(); i++) {
            os << *(parseTree.children[i]);
        }
    } else {
        os << '\n';
    }
    return os;
}
