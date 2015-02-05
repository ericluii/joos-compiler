#ifndef __WEEDS_NO_STATIC_FINAL_NATIVE_INTERFACE_METHOD_H__
#define __WEEDS_NO_STATIC_FINAL_NATIVE_INTERFACE_METHOD_H__

#include "weed.h"
#include <cassert>

class NoStaticFinalNativeProtectedInterfaceMethod : public Weed
{
    public:
        NoStaticFinalNativeProtectedInterfaceMethod()
        {
            rule = ABSTRACT_METHOD_UNROLL;
        }

        unsigned int hasMod(unsigned int rule, ParseTree* node) {
            unsigned int found = 0;

            switch (node->rule) {
                case ABSTRACT_METHOD_UNROLL:
                case ABSTRACT_METHOD_TYPE:
                case ABSTRACT_METHOD_VOID:
                case MEMBER_MOD_EXPAND:
                case MEMBER_MOD_EPSILON:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasMod(rule, node->children[i]);
                    }
                    break;
                default:
                    return rule == (unsigned int) node->rule;
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
                std::stringstream ss;
                ss << "Interface method '" << getMethodName(node) << "' cannot be declared as static.";

                Error(E_WEEDER, token, ss.str());
            }

            if (hasMod(MEMBER_MOD_FINAL, node)) {
                std::stringstream ss;
                ss << "Interface method '" << getMethodName(node) << "' cannot be declared as final.";

                Error(E_WEEDER, token, ss.str());
            }

            if (hasMod(MEMBER_MOD_NATIVE, node)) {
                std::stringstream ss;
                ss << "Interface method '" << getMethodName(node) << "' cannot be declared as native.";

                Error(E_WEEDER, token, ss.str());
            }

            if (hasMod(MEMBER_MOD_PROTECTED, node)) {
                std::stringstream ss;
                ss << "Interface method '" << getMethodName(node) << "' cannot be declared as protected.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
