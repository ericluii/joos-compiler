#ifndef __WEEDS_NO_ABSTRACT_BODY_H__
#define __WEEDS_NO_ABSTRACT_BODY_H__

#include "weed.h"
#include <cassert>

class NoAbstractBody final : public Weed
{
    public:
        NoAbstractBody()
        {
            rule = METHOD_HEADER_AND_BODY;
        }

        int hasAbstractMod(ParseTree* node) {
            int found = 0;

            switch (node->rule) {
                case METHOD_HEADER_AND_BODY:
                case METHOD_TYPE:
                case METHOD_VOID:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for (int i = 0; i < node->children.size(); i++) {
                        found += hasAbstractMod(node->children[i]);
                    }
                    break;
                case MEMBER_MOD_ABSTRACT:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        int check(ParseTree* node)
        {
            if (hasAbstractMod(node)) {
                for (int i = 0; i < node->children.size(); i++) {
                    if (node->children[i]->rule == METHOD_BODY_EMPTY) {
                        return 0;
                    } else if (node->children[i]->rule == METHOD_BODY) {
                        return 1;
                    }
                }

                // Should never get here if abstract class
                assert(false);
            }

            return 0;
        }
};

#endif
