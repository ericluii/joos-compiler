#ifndef __WEEDS_NO_FINAL_OR_STATIC_IN_ABSTRACT_METHOD_H__
#define __WEEDS_NO_FINAL_OR_STATIC_IN_ABSTRACT_METHOD_H__

#include "weed.h"
#include <cassert>

class NoFinalStaticAbstractMethod : public Weed
{
    public:
        NoFinalStaticAbstractMethod()
        {
            rule = METHOD_HEADER_AND_BODY;
        }

        unsigned int hasMod(unsigned int rule, ParseTree* node) {
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
                    return rule == (unsigned int)node->rule;
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

        unsigned int check(ParseTree* node)
        {
            if (hasMod(MEMBER_MOD_ABSTRACT, node)) {
                if (hasMod(MEMBER_MOD_STATIC, node)) {
                        std::cerr << "Weeding error in file: TODO" << std::endl;
                        std::cerr << "Abstract method '" << getMethodName(node) << "' cannot be declared as static." << std::endl;
                    return 1;
                }

                if (hasMod(MEMBER_MOD_FINAL, node)) {
                        std::cerr << "Weeding error in file: TODO" << std::endl;
                        std::cerr << "Abstract method '" << getMethodName(node) << "' cannot be declared as final." << std::endl;
                    return 1;
                }
            }

            return 0;
        }
};

#endif
