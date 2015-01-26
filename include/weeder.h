#ifndef __WEEDER_H__
#define __WEEDER_H__

#include "parseTree.h"
#include "weeds_impl.h"
#include <map>

class Weeder final
{
    private:
        bool didGenSymbolTable;
        std::map<std::string, std::string> &symbolTable;
        std::vector<Weed *> weeds;
    public:
        Weeder(std::map<std::string, std::string> &st);
        int buildSymbolTable(ParseTree* node);
        int weedParseTree(ParseTree* node);

        ~Weeder()
        {
            for (int i = 0; i < weeds.size(); i++) {
                delete weeds[i];
            }
        }
};

#endif
