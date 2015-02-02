#include "parser.h"
#include "ruleNumbers.h"
#include "parserRulesAndActions.h"
#include <sstream>
#include <iostream>
#include <cassert>

#define START_STATE 0

void Parser::pushSymbolStack(ParseTree* newTree) {
    if(curLocStack == symbolStack.size()) {
        symbolStack.push_back(newTree);
    } else {
        symbolStack[curLocStack] = newTree;
    }
    curLocStack+= 1;
}

void Parser::createNonTerminalNode(int ruleToReduce) {
    ParseTree* newTree = new ParseTree(NULL, ruleToReduce);
    unsigned int numRHS = rules[ruleToReduce].size() - 1;
    for(unsigned int i = numRHS; i > 0; i--) {
        stateStack.pop();
        newTree->addChild(symbolStack[curLocStack-i]);
    }
    curLocStack-= numRHS;
    pushSymbolStack(newTree);
}

void Parser::printExpectedTokens(int state) {
    int counter = 1;
    int entrySize = parserTable[state].size();
    std::cerr << "Expected tokens include:\n";
    std::map<std::string, std::pair<std::string, int> >::iterator it;
    for(it = parserTable[state].begin(); it != parserTable[state].end(); it++) {
        std::string token = it->first;
        if(token == "ID") {
            std::cerr << "an identifier";
        } else if(token == "CHAR") {
            std::cerr << "a char literal";
        } else if(token == "STRING") {
            std::cerr << "a string literal";
        } else if(token == "NUMBER") {
            std::cerr << "an integer";
        } else {
            // checks if it isn't a non-terminal i.e a terminal
            if(!('A' <= token[0] && token[0] <= 'Z')) {
                std::cerr << token;
            } else {
                counter++;
                continue;
            }
        }

        if(counter != entrySize) {
            std::cerr << ", ";
            if(counter == entrySize - 1) {
                std::cerr << "and ";
            }
        }
        counter++;
    }
    std::cerr << '\n' << std::endl;
}

bool Parser::checkParsingCompletion(int lastState, std::string& fileName) {
    unsigned int symbolStackSize = symbolStack.size();
    if(symbolStackSize != 3 || symbolStackSize != 4) {
        if(symbolStackSize == 1) {
            if(symbolStack[0]->rule == COMPILATION_UNIT_CLASS ||
               symbolStack[0]->rule == COMPILATION_UNIT_INTERFACE) {
                return true;
            }
        }
        return false;
    }
    
    int firstNodeRule = symbolStack[0]->rule;
    int secondNodeRule = symbolStack[1]->rule;
    int thirdNodeRule = symbolStack[2]->rule;

    if((firstNodeRule != PACKAGE_NAME && firstNodeRule != PACKAGE_EPSILON) &&
       (secondNodeRule != IMPORT_STAR_DECLS && secondNodeRule != IMPORT_STAR_EPSILON) &&
       (thirdNodeRule != CLASS_DECL && thirdNodeRule != INTERFACE_DECL)) {
        std::cerr << "Incomplete file: " << fileName << " was given. ";
        printExpectedTokens(lastState);
        return false;
    }
    return true;
}

Parser::Parser(std::map<std::string, std::vector<Token*> *>& setTokens) : tokens(setTokens), curLocStack(0) {
    numRules = NUM_RULES;
    numStates = NUM_STATES;
    initParser();
}

void Parser::initParser() {
    for(int i = 0; i < numRules; i++) {
        std::string partRule;
        std::vector<std::string> rule;
        std::istringstream iss(rulesAndActions[i]);
        while(iss >> partRule) {
            rule.push_back(partRule);
        }
        rules.push_back(rule);
    }

    parserTable.resize(numStates);
    
    int numActions = NUM_ACTIONS;

    int stateNumber;
    std::string input;
    std::string action;
    int ruleOrStateNum;

    for(int i = numRules; i < numActions+numRules; i++) {
        std::istringstream iss(rulesAndActions[i]);
        iss >> stateNumber >> input >> action >> ruleOrStateNum;
        parserTable[stateNumber][input] = make_pair(action, ruleOrStateNum);
    }
}

void Parser::printErrorStatement(Token* token, int curState) {
    std::pair<unsigned int, unsigned int> location = token->getLocation();
    std::cerr << "An unexpected token: " << token->getString() << " was seen at: "
              << location.first << "(row), " << location.second << "(column)" << std::endl;
    printExpectedTokens(curState);
}

void Parser::resetParser(bool success) {
    if(success) {
        assert(symbolStack.size() > 0);
        assert(symbolStack[0]->token->getString() == "CompilationUnit");
    }

    curLocStack = 0;
    for(unsigned int i = 0; i < stateStack.size(); i++) {
        stateStack.pop();
    }
}

ParseTree* Parser::Parse(std::string& parseFile) {
    std::vector<Token*>* fileTokens = tokens[parseFile];
    std::string nonTerminal;
    int curState = START_STATE;
    stateStack.push(curState);
    Token* token;
    for(unsigned int tokensIndex = 0; tokensIndex < fileTokens->size();) {
        token = (*fileTokens)[tokensIndex];
        TOKEN_TYPE tokType = token->getType();
        std::string toParse;
        if(tokType == TT_ID) {
            toParse = "ID";
        } else if(tokType == TT_CHARACTER) {
            toParse = "CHAR";
        } else if(tokType == TT_STRING) {
            toParse = "STRING";
        } else if(tokType == TT_NUM) {
            toParse = "NUMBER";
        } else {
            toParse = token->getString();
        }

        // invalid token
        if(parserTable[curState].find(toParse) == parserTable[curState].end()) {
            std::cerr << "Parsing error in file: " << parseFile << "\n";
            printErrorStatement(token, curState);
            resetParser(false);
            return NULL;
        }

        if(parserTable[curState][toParse].first == "shift") {
            curState = parserTable[curState][toParse].second;
            pushSymbolStack(new ParseTree(token, 0));
            stateStack.push(curState);
            tokensIndex++;
        } else {
            int ruleToReduce;
            int round = 0;
            do {
                ruleToReduce = parserTable[curState][toParse].second;
                createNonTerminalNode(parserTable[curState][toParse].second);
                curState = stateStack.top();
                nonTerminal = rules[ruleToReduce][0];
                if(parserTable[curState].find(nonTerminal) == parserTable[curState].end() ||
                   parserTable[curState][nonTerminal].first != "shift") {
                    std::cerr << "Parsing error in file: " << parseFile << "\n";
                    printErrorStatement(token, curState);
                    resetParser(false);
                    return NULL;
                }

                curState = parserTable[curState][nonTerminal].second;
                stateStack.push(curState);
                round++;
            } while(parserTable[curState][toParse].first == "reduce");
        }
    }

    if(checkParsingCompletion(curState, parseFile)) {
        int firstNodeRule = symbolStack[0]->rule;
        int thirdNodeRule = symbolStack[2]->rule;
        // Cause to push to the new node to the bottom of the stack
        curLocStack = 0;
        if(firstNodeRule != COMPILATION_UNIT_CLASS && firstNodeRule != COMPILATION_UNIT_INTERFACE) { 
            if(thirdNodeRule == CLASS_DECL) {
                createNonTerminalNode(COMPILATION_UNIT_CLASS);
            } else {
                createNonTerminalNode(COMPILATION_UNIT_INTERFACE);
            }
        }
        
        resetParser(true);
        return symbolStack[0];
    }
    
    resetParser(false);
    return NULL;
}
