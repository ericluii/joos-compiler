#ifndef __WEEDS_H__
#define __WEEDS_H__

#include "parseTree.h"
#include "ruleNumbers.h"

class Weed
{
    public:
        int rule;
        virtual unsigned int check(ParseTree* node) = 0;
        virtual bool checkRule(int check) {
            return rule == check;
        }

        virtual ~Weed() {}
};

#endif
