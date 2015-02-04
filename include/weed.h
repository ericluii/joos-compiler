#ifndef __WEEDS_H__
#define __WEEDS_H__

#include "parseTree.h"
#include "ruleNumbers.h"

class Weed
{
    public:
        int rule;
        virtual int check(ParseTree* node) = 0;

        virtual ~Weed() {}
};

#endif
