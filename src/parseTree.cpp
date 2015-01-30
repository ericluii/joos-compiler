#include "parseTree.h"

ParseTree::ParseTree(Token* token, int ruleNum) : rule(ruleNum), token(token) {}

void ParseTree::addChild(ParseTree* child) {
    children.push_back(child);
}
