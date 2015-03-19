#include "parser.h"
#include "error.h"
#include "ruleNumbers.h"
#include "parserRules.h"
#include "parserActions.h"
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
    curLocStack++;
}

void Parser::createNonTerminalNode(int ruleToReduce) {
    ParseTree* newTree = new ParseTree(NULL, ruleToReduce, rules[ruleToReduce][0]);
    unsigned int numRHS = rules[ruleToReduce].size() - 1;
    for(unsigned int i = numRHS; i > 0; i--) {
        stateStack.pop();
        newTree->addChild(symbolStack[curLocStack-i]);
    }
    curLocStack-= numRHS;
    pushSymbolStack(newTree);
}

bool Parser::isTerminal(char c) {
    return (!('A' <= c && c <= 'Z') && c != '$');
}

void Parser::getExpectedTokens(int state, std::stringstream &ss) {
    int counter = 1;
    int entrySize = parserTable[state].size();
    ss << "\n\tExpected tokens include: ";
    std::map<std::string, std::pair<std::string, int> >::iterator it;
    for(it = parserTable[state].begin(); it != parserTable[state].end(); it++) {
        std::string token = it->first;
        if(counter <= entrySize && counter != 1) {
            if(isTerminal(token[0]) || token == "ID" || token == "CHAR" ||
               token == "STRING" || token == "NUMBER") { 
                ss << ", ";
                if(counter == entrySize) {
                    ss << "and ";
                }
            }
        }
        counter++;
        
        if(token == "ID") {
            ss << "an identifier";
        } else if(token == "CHAR") {
            ss << "a char literal";
        } else if(token == "STRING") {
            ss << "a string literal";
        } else if(token == "NUMBER") {
            ss << "an integer";
        } else {
            // checks if it isn't a non-terminal i.e a terminal
            if(isTerminal(token[0])) {
                ss << '"' << token << '"';
            } else {
                counter++;
                continue;
            }
        }
    }
}

bool Parser::checkParsingCompletion(int lastState, std::string& fileName) {
    unsigned int symbolStackSize = symbolStack.size();
    if(symbolStackSize > 0) {
        if(symbolStack[0]->rule == COMPILATION_UNIT) {
            return true;
        } else if (symbolStackSize >= 3) {
            int firstNodeRule = symbolStack[0]->rule;
            int secondNodeRule = symbolStack[1]->rule;
            int thirdNodeRule = symbolStack[2]->rule;

            if((firstNodeRule != PACKAGE_NAME && firstNodeRule != PACKAGE_EPSILON) ||
               (secondNodeRule != IMPORT_STAR_DECLS && secondNodeRule != IMPORT_STAR_EPSILON) ||
               (thirdNodeRule != TYPE_CLASS && thirdNodeRule != TYPE_INTERFACE && thirdNodeRule != TYPE_EPSILON)) {
                std::stringstream ss;
                ss << "COULD NOT GENERATE PARSE TREE DUE TO PARSER MALFUNCTION. TELL DEVELOPERS";
                getExpectedTokens(lastState, ss);
                cleanSymbolStack();

                Error(E_DEFAULT, NULL, ss.str());
                return false;
            }
            return true;
        }
    }
    return false;
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
        std::istringstream iss(parserRules[i]);
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

    for(int i = 0; i < numActions; i++) {
        std::istringstream iss(parserActions[i]);
        iss >> stateNumber >> input >> action >> ruleOrStateNum;
        parserTable[stateNumber][input] = make_pair(action, ruleOrStateNum);
    }
}

void Parser::getErrorMessage(Token* token, int curState, std::stringstream &ss) {
    if(token->getType() == TT_EOF) {
        ss << "An incomplete file was given.";
    } else {
        ss << "An unexpected token: '" << token->getString() << "' was found.";
    }
    getExpectedTokens(curState, ss);
}

void Parser::resetParser(bool success) {
    if(success) {
        assert(symbolStack.size() > 0);
        assert(symbolStack[0]->rule == COMPILATION_UNIT);
    }

    curLocStack = 0;
    for(unsigned int i = stateStack.size(); i > 0; i--) {
        stateStack.pop();
    }
}

void Parser::cleanSymbolStack() {
    for(unsigned int i = 0; i < curLocStack; i++) {
        delete symbolStack[i];
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
            if(token->getString() == "false" || token->getString() == "true") {
                // true and false was being set to ID -> incorrect
                toParse = token->getString();
            } else {
                toParse = "ID";
            }
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
            std::stringstream ss;
            getErrorMessage(token, curState, ss);
            resetParser(false);
            cleanSymbolStack();

            Error(E_PARSER, token, ss.str());
            return NULL;
        }

        if(parserTable[curState][toParse].first == "shift") {
            curState = parserTable[curState][toParse].second;
            pushSymbolStack(new ParseTree(token, GOAL, toParse));
            stateStack.push(curState);
            tokensIndex++;
        } else {
            int ruleToReduce;
            do {
                ruleToReduce = parserTable[curState][toParse].second;
                createNonTerminalNode(parserTable[curState][toParse].second);
                curState = stateStack.top();
                nonTerminal = rules[ruleToReduce][0];
                if(parserTable[curState].find(nonTerminal) == parserTable[curState].end() ||
                   parserTable[curState][nonTerminal].first != "shift") {
                    std::stringstream ss;
                    getErrorMessage(token, curState, ss);
                    resetParser(false);
                    cleanSymbolStack();

                    Error(E_PARSER, token, ss.str());
                    return NULL;
                }
                
                curState = parserTable[curState][nonTerminal].second;
                stateStack.push(curState);
            } while(parserTable[curState][toParse].first == "reduce");
        }
    }
  
    if(checkParsingCompletion(curState, parseFile)) {
        int firstNodeRule = symbolStack[0]->rule;
        // Cause to push the new node to the bottom of the stack
        if(firstNodeRule != COMPILATION_UNIT) { 
            curLocStack = rules[COMPILATION_UNIT].size() - 1;
            createNonTerminalNode(COMPILATION_UNIT);
        }
       
        resetParser(true);
        return symbolStack[0];
    }

    resetParser(false);
    cleanSymbolStack();
    return NULL;
}
