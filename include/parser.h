#ifndef __PARSER_H__
#define __PARSER_H__

#include "token.h"
#include "parseTree.h"
#include <map>
#include <string>
#include <vector>
#include <stack>

class Parser final {
    private:
        std::map<std::string, std::vector<Token*> *>& tokens;
        unsigned int curLocStack;
        int numRules;
        int numStates;

        std::vector<ParseTree*> symbolStack;
        std::stack<int> stateStack;
        std::vector<std::vector<std::string> > rules;
        std::vector<std::map<std::string, std::pair<std::string, int> > > parserTable;

        void pushSymbolStack(ParseTree* newTree);
        void createNonTerminalNode(int ruleToReduce);
        bool checkParsingCompletion(int lastState, std::string& fileName);
        void printErrorStatement(Token* token, int curState);
        void printExpectedTokens(int state);
        void resetParser(bool success);
    public:
        void initParser();
        ParseTree* Parse(std::string& parseFile);
        Parser(std::map<std::string, std::vector<Token*> *>& setTokens);
};

#endif
