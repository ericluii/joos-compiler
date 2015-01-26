#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "dfa.h"
#include "token.h"
#include <vector>
#include <fstream>

class Scanner final
{
    private:
        std::vector<Dfa*> dfas;
    public:
        int Scan(std::ifstream& file, std::vector<Token*> *tokens);
        Scanner();

        ~Scanner()
        {
            for (int i = 0; i < dfas.size(); i++) {
                delete dfas[i];
            }
        }
};

#endif

