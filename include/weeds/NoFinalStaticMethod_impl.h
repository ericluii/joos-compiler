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

        unsigned int hasMod(int rule, ParseTree* node) {
            unsigned int found = 0;

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
                    token = node->children[i]->children[0]->token;
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
        }

        void check(ParseTree* node)
        {
            if (hasMod(MEMBER_MOD_STATIC, node)) {
                if (hasMod(MEMBER_MOD_FINAL, node)) {
                    std::stringstream ss;
                    ss << "Static method '" << getMethodName(node) << "' cannot be declared as final.";

                    Error(E_WEEDER, token, ss.str());
                }
            }
        }
};

#endif
