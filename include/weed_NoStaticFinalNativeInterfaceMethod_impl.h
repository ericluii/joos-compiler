#ifndef __WEEDS_NO_STATIC_FINAL_NATIVE_INTERFACE_METHOD_H__
#define __WEEDS_NO_STATIC_FINAL_NATIVE_INTERFACE_METHOD_H__

#include "weed.h"
#include <cassert>

class NoStaticFinalNativeInterfaceMethod final : public Weed
{
    public:
        NoStaticFinalNativeInterfaceMethod()
        {
            rule = ABSTRACT_METHOD_UNROLL;
        }

        int hasMod(int rule, ParseTree* node) {
            int found = 0;

            switch (node->rule) {
                case ABSTRACT_METHOD_UNROLL:
                case METHOD_TYPE:
                case METHOD_VOID:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for (int i = 0; i < node->children.size(); i++) {
                        found += hasMod(rule, node->children[i]);
                    }
                    break;
                default:
                    return rule == node->rule;
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
            if (hasMod(MEMBER_MOD_STATIC, node)) {
                    std::cerr << "Weeding error in file: TODO" << std::endl;
                    std::cerr << "Interface method '" << getMethodName(node) << "' cannot be declared as static." << std::endl;
                return 1;
            }

            if (hasMod(MEMBER_MOD_FINAL, node)) {
                    std::cerr << "Weeding error in file: TODO" << std::endl;
                    std::cerr << "Interface method '" << getMethodName(node) << "' cannot be declared as final." << std::endl;
                return 1;
            }

            if (hasMod(MEMBER_MOD_NATIVE, node)) {
                    std::cerr << "Weeding error in file: TODO" << std::endl;
                    std::cerr << "Interface method '" << getMethodName(node) << "' cannot be declared as native." << std::endl;
                return 1;
            }

            return 0;
        }
};

#endif
