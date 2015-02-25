#ifndef __NO_MULTI_ARRAY_ACCESS_H__
#define __NO_MULTI_ARRAY_ACCESS_H__

#include "weed.h"

class NoMultiArrayAccess : public Weed {
    public:
        NoMultiArrayAccess() {
            rule = ACCESS_ARRAY_PRIMARY;
        }

        void check(ParseTree* node) {
            if(node->children[0]->rule == PRIMARY_ARRAY_ACCESS) {
                std::stringstream ss;
                ss << "Accessing a multidimensional array is not allowed.";
                Error(E_WEEDER, node->children[1]->token, ss.str());
            }
        }
};

#endif
