#ifndef __WEEDS_NO_FINAL_STATIC_METHOD_H__
#define __WEEDS_NO_FINAL_STATIC_METHOD_H__

#include "weed.h"
#include <cassert>

class NoFinalStaticMethod : public Weed
{
    public:
        NoFinalStaticMethod()
        {
            rule = METHOD_HEADER_AND_BODY;
        }

        int hasMod(int rule, ParseTree* node) {
            int found = 0;

            switch (node->rule) {
                case METHOD_HEADER_AND_BODY:
                case METHOD_TYPE:
                case METHOD_VOID:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasMod(rule, node->children[i]);
                    }
                    break;
                default:
                    return rule == node->rule;
            }

            return found;
        }

        std::string getMethodName(ParseTree* node) {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == METHOD_TYPE ||
                    node->children[i]->rule == METHOD_VOID) {
                    node = node->children[i];
                    break;
                }
            }

            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == METHOD_DECL) {
                    node = node->children[i];
                    break;
                }
            }

            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
        }

        int check(ParseTree* node)
        {
            if (hasMod(MEMBER_MOD_FINAL, node)) {
                if (hasMod(MEMBER_MOD_FINAL, node)) {
                        std::cerr << "Weeding error in file: TODO" << std::endl;
                        std::cerr << "Static method '" << getMethodName(node) << "' cannot be declared as final." << std::endl;
                    return 1;
                }
            }

            return 0;
        }
};

#endif
