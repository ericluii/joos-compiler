#ifndef __WEED_NO_PUBLIC_AND_PROTECTED_H__
#define __WEED_NO_PUBLIC_AND_PROTECTED_H__

#include "weed.h"

class NoPublicAndProtected : public Weed {
    public:
        unsigned int hasMod(int rule, ParseTree* node) {
            unsigned int found = 0;
            
            switch(node->rule) {
                case FIELD_DECL:
                case METHOD_HEADER_AND_BODY:
                case METHOD_TYPE:
                case METHOD_VOID:
                case CONSTRUCTOR_PARTS:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for(unsigned int i = 0; i < node->children.size(); i++) {
                        found+= hasMod(rule, node->children[i]);
                    }

                    break;
                default:
                    return rule == node->rule;
            }

            return found;
        }
};

#endif
