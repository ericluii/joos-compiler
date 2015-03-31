#include "scanner.h"
#include "error.h"
#include "dfas/identifierDfa.h"
#include "dfas/validNumberDfa.h"
#include "dfas/operatorDfa.h"
#include "dfas/separatorDfa.h"
#include "dfas/charStringLiteralDfa.h"
#include "dfas/keywordDfa.h"
#include "dfas/singleCommentDfa.h"
#include "dfas/multiCommentDfa.h"
#include "dfas/whitespaceAndControlDfa.h"
#include "dfas/bannedKeywordDfa.h"
#include "dfas/bannedOperatorDfa.h"

#include <iostream>
#include <cassert>
#include <sstream>

Scanner::Scanner()
{
    // Dfas must be ordered in reverse priority.
    // Dfas that are later in the list will have priority over earlier Dfas
    dfas.push_back(new WhitespaceAndControlDfa());
    dfas.push_back(new SingleCommentDfa());
    dfas.push_back(new MultiCommentDfa());
    dfas.push_back(new IdentifierDfa());
    dfas.push_back(new ValidNumberDfa());
    dfas.push_back(new OperatorDfa());
    dfas.push_back(new SeparatorDfa());
    dfas.push_back(new CharStringLiteralDfa());
    dfas.push_back(new KeywordDfa());
    dfas.push_back(new BannedKeywordDfa());
    dfas.push_back(new BannedOperatorDfa());
}

int Scanner::Scan(std::ifstream& file, std::vector<Token*> *tokens)
{
    int c = -2;

    int numDfas = this->dfas.size();
    int errorCount = 0;
    std::vector<int> errorFlags(numDfas, 0);
    std::pair<DFA_STATES, TOKEN_TYPE> result;
    bool escapeSequence = false;
    bool doneSequence = false;

    //Values for the token currently being scanned
    TOKEN_TYPE type = TT_INVALID;
    std::string lexime = "";
    int tokenLine = 1; 
    int tokenCollumn = 1; 
    
    int currentLine = 1;
    int currentColumn = 0;
    
    while (file) {
        if (c == -2) {
            c = file.get();

            // EOF is represented as -1
            // ASCII is from 0 to 127
            if (c < -1 || c > 127) {
                std::stringstream ss;
                ss << fileName << ":" << currentLine << ":" << currentColumn
                   << ": error: Invalid character with ASCII CODE: " << c << ".";

                Error(E_SCANNER, NULL, ss.str());
                return SCANNER_NON_ASCII;
            }

            // Line feed character LF
            if(c == 10) {
                currentLine++;
                currentColumn = -1;
            }
            currentColumn++;
        }

        // Dfas must be ordered in reverse priority.
        // Dfas that are later in the list will have priority over earlier Dfas
        for (int i = 0; i < numDfas; i++) {
            std::stringstream ss;
            if (errorFlags[i] == 0) {
                result = dfas[i]->transition((char)c);
                switch (result.first) {
                    case DS_ESCAPESINGLE:
                    case DS_ESCAPEDOUBLE:
                        escapeSequence = true;
                        break;
                    case DS_ENCLOSESINGLE:
                    case DS_DOUBLEQUOTE:
                        doneSequence = true;
                        lexime += CharStringLiteralDfa::replacement;
                        break;
                    case DS_ACCEPT:
                        type = result.second;
                        break;
                    case DS_ERROR:
                        errorFlags[i] = 1;
                        errorCount++;
                        break;
                    case DS_ABORT:
                        type = result.second;
                        break;
                    default:
                        break;
                }
            }
        }

        // Sanity Check
        assert(errorCount <= numDfas);

        if (errorCount == numDfas) {
            if(type == TT_INVALID){
                std::stringstream ss;
                ss << fileName << ":" << currentLine << ":" << currentColumn << ": error: Invalid token with lexime: " << lexime << (char)c << ".";

                Error(E_SCANNER, NULL, ss.str());
                return SCANNER_INV_LEXEME;
            }

            if(type != TT_COMMENT && type != TT_WHITESPACE) {
                tokens->push_back(new Token(type, lexime, std::pair <unsigned int, unsigned int>(tokenLine, tokenCollumn), fileName));
            }

            if(type == TT_MINUSMINUS || type == TT_PLUSPLUS || type == TT_DO || type == TT_TRY ||
               type == TT_CASE || type == TT_GOTO || type == TT_LONG || type == TT_CATCH || type == TT_CONST ||
               type == TT_CATCH || type == TT_CONST || type == TT_FLOAT || type == TT_SUPER || type == TT_THROW ||
               type == TT_DOUBLE || type == TT_SWITCH || type == TT_THROWS || type == TT_DEFAULT || type == TT_FINALLY ||
               type == TT_FINALLY || type == TT_PRIVATE || type == TT_CONTINUE || type == TT_VOLATILE || type == TT_STRICTFP ||
               type == TT_TRANSIENT || type == TT_SYNCHRONIZED) {
                std::stringstream ss;
                ss << fileName << ":" << currentLine << ":" << currentColumn << ": error: Unsupported Java keyword: " << lexime << (char)c << ".";

                Error(E_SCANNER, NULL, ss.str());
                return SCANNER_INV_LEXEME;

            }

            // Reset all the things!!!
            type = TT_INVALID;
            lexime = "";
            errorCount = 0;
            tokenLine = currentLine;
            tokenCollumn = currentColumn;

            for(int i = 0; i < numDfas; i++) {
                dfas[i]->resetDfa();
                errorFlags[i] = 0;
            }
        } else {
            if(!escapeSequence){
                lexime += (char)c;
            }
            if(doneSequence == true){
                escapeSequence = false;
                doneSequence = false;
            }
            c = -2;
        }
    }

    // Indicate EOF
    tokens->push_back(new Token(TT_EOF, "$", std::pair<unsigned int, unsigned int>(tokenLine, tokenCollumn+1), fileName));
    return SCANNER_OK;
}

void Scanner::resetDFAs() {
    for(unsigned int i = 0; i < dfas.size(); i++) {
        dfas[i]->resetDfa();
    }
}

void Scanner::setFileName(std::string setName) {
    fileName = setName;
}
