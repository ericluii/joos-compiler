#ifndef __WEEDER_H__
#define __WEEDER_H__

#include "parseTree.h"
#include "weeds_impl.h"
#include <map>

class Weeder final
{
    private:
        std::vector<Weed *> weeds;
    public:
        Weeder();
        int weedParseTree(ParseTree* node);

        ~Weeder()
        {
            for (int i = 0; i < weeds.size(); i++) {
                delete weeds[i];
            }
        }
};

#endif
