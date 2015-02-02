#ifndef __WEED_NON_ABSTRACT_BODY_H__
#define __WEED_NON_ABSTRACT_BODY_H__

#include "weed.h"
#include <cassert>

class NonAbstractBody final : public Weed
{
    public:
        NonAbstractBody()
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

        std::string getMethodName(ParseTree* node) {
            for (int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == METHOD_TYPE ||
                    node->children[i]->rule == METHOD_VOID) {
                    node = node->children[i];
                    break;
                }
            }

            for (int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == METHOD_DECL) {
                    node = node->children[i];
                    break;
                }
            }

            for (int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
        }

        int check(ParseTree* node)
        {
            if (!hasAbstractMod(node)) {
                for (int i = 0; i < node->children.size(); i++) {
                    if (node->children[i]->rule == METHOD_BODY_EMPTY) {
                        std::cerr << "Weeding error in file: TODO" << std::endl;
                        std::cerr << "Non-Abstract method '" << getMethodName(node) << "' must have a body." << std::endl;
                        return 1;
                    } else if (node->children[i]->rule == METHOD_BODY) {
                        return 0;
                    }
                }

                assert(false);
            }

            return 0;
        }
};

#endif
