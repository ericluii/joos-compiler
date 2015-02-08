#ifndef __WEEDS_H__
#define __WEEDS_H__

#include "parseTree.h"
#include "ruleNumbers.h"
#include "error.h"
#include <sstream>

class Weed
{
    protected:
        Token* token;
    public:
        int rule;
        static std::string className;
        static std::string interfaceName;
        virtual void check(ParseTree* node) = 0;
        virtual bool checkRule(int check) {
            return rule == check;
        }

        virtual ~Weed() {}
};


#endif
