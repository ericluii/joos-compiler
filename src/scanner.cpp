#include "scanner.h"
#include "identifierDfa.h"
#include "validNumberDfa.h"
#include "operatorDfa.h"
#include "separatorDfa.h"
#include "charStringLiteralDfa.h"
#include "keywordDfa.h"
#include "singleCommentDfa.h"
#include "multiCommentDfa.h"

#include <iostream>
#include <cassert>
#include <string>

Scanner::Scanner()
{
    // Dfas must be ordered in reverse priority.
    // Dfas that are later in the list will have priority over earlier Dfas
    std::vector<Dfa*> dfas;
    dfas.push_back(new SingleCommentDfa());
    dfas.push_back(new MultiCommentDfa());
    dfas.push_back(new IdentifierDfa());
    dfas.push_back(new ValidNumberDfa());
    dfas.push_back(new OperatorDfa());
    dfas.push_back(new SeparatorDfa());
    dfas.push_back(new CharStringLiteralDfa());
    dfas.push_back(new KeywordDfa());
}

std::vector<Token*> *Scanner::Scan(std::ifstream& file)
{
    std::vector<Token*> *tokens =  new std::vector<Token*>();
    int c = -2;
    
    int numDfas = this->dfas.size();
    int errorCount = 0;
    std::vector<int> errorFlags(numDfas, 0);
    std::pair<DFA_STATES, TOKEN_TYPE> result;
    
    //Values for the token currently being scanned
    TOKEN_TYPE type = TT_INVALID;
    std::string lexime = "";
    int tokenLine = 0; 
    int tokenCollumn = 0; 
    
    int currentLine = 0;
    int currentColumn = -1;
    
    while (file) {
        if (c == -2) {
            c = file.get();
            currentColumn++;
    
            // EOF is represented as -1
            // ASCII is from 0 to 127
            if (c < -1 || c > 127) {
                std::cerr << "Invalid character with code: " 
                          << c 
                          << "\n Input files must contain only ASCII characters";
            }

            // Line feed character LF
            if(c == 10) {
                currentLine++;
                currentColumn = -1;
            }
        }

        // Dfas must be ordered in reverse priority.
        // Dfas that are later in the list will have priority over earlier Dfas
        for (int i = 0; i < numDfas; i++) {
            if (errorFlags[i] == 0) {
                result = dfas[i]->transition((char)c);
                switch (result.first) {
                    case DS_ACCEPT:
                        type = result.second;
                        break;
                    case DS_ERROR:
                        errorFlags[i] = 1;
                        errorCount++;
                        break;
                    default:
                        break;
                }
            }
        }

        // Sanity Check
        assert(errorCount <= numDfas); 

        if (errorCount == numDfas) {
            if(type != TT_INVALID && type != TT_COMMENT) {
                tokens->push_back(new Token(type, lexime, std::pair <unsigned int, unsigned int>(tokenLine, tokenCollumn)));
            }

            // Reset all the things!!!
            type = TT_INVALID;
            lexime = "";
            errorCount = 0;    
            tokenLine = currentLine;
            tokenCollumn = currentColumn;

            // Eliminates whitespace and control characters.
            // This seemed more reasonable than creating an entire dfa for the same purpose
            if(c <= 32 || c == 127) {
                c = -2;
            }

            for(int i = 0; i < numDfas; i++) {
                dfas[i]->resetDfa();
                errorFlags[i] = 0;
            }
        } else {
            lexime += (char)c;
            c = -2;
        }
    }
    
    return tokens;
}
