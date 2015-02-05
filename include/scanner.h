#ifndef __SCANNER_H__
#define __SCANNER_H__

#define SCANNER_OK 0
#define SCANNER_NON_ASCII 1
#define SCANNER_INV_LEXEME 2
#define SCANNER_ABORT 3

#include "dfa.h"
#include "token.h"
#include <vector>
#include <fstream>
#include <string>

class Scanner
{
    private:
        std::string fileName;
        std::vector<Dfa*> dfas;
    public:
        int Scan(std::ifstream& file, std::vector<Token*> *tokens);
        void resetDFAs();
        void setFileName(std::string setName);
        Scanner();

        ~Scanner()
        {
            for (unsigned int i = 0; i < dfas.size(); i++) {
                delete dfas[i];
            }
        }
};

#endif

