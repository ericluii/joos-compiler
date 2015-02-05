#ifndef __WEEDER_H__
#define __WEEDER_H__

#include "parseTree.h"
#include "weed.h"
#include <map>

class Weeder
{
    private:
        std::vector<Weed *> weeds;
    public:
        Weeder();
        void weedParseTree(ParseTree* node);

        ~Weeder()
        {
            for (unsigned int i = 0; i < weeds.size(); i++) {
                delete weeds[i];
            }
        }
};

#endif
