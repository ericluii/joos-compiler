#ifndef __RESTRICTED_NATIVE_METHOD_H__
#define __RESTRICTED_NATIVE_METHOD_H__

#include "weed.h"
#include <cassert>

class RestrictedNativeMethod : public Weed
{
    public:
        RestrictedNativeMethod()
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

        bool intReturnType(ParseTree* node) {
            if(node->children[0]->children[1]->rule == PRIMITIVE_TYPE) {
                return node->children[0]->children[1]->children[0]->rule == TYPE_INT;
            }

            return false;
        }

        bool oneParameterIntType(ParseTree* node) {
            while(true) {
                switch(node->rule) {
                    case METHOD_TYPE:
                    case METHOD_VOID:
                    case METHOD_DECL:
                        node = node->children[2];
                        break;
                    case METHOD_HEADER_AND_BODY:
                    case FORMAL_PARAMSTAR:
                    case FORMAL_PARAM:
                    case PARAMETER_EXPAND:
                    case PRIMITIVE_TYPE:
                        node = node->children[0];
                        break;
                    case TYPE_INT:
                        return 1;
                    default:
                        return 0;
                }
            }

            return 0;
        }


        void check(ParseTree* node)
        {
            unsigned int hasNative = hasMod(MEMBER_MOD_NATIVE, node);
            if (hasNative && !hasMod(MEMBER_MOD_STATIC, node)) {
                std::stringstream ss;
                ss << "Native method '" << getMethodName(node) << "' must be declared as static.";

                Error(E_WEEDER, token, ss.str());
            } else if (hasNative){
                if (!intReturnType(node)) {
                    std::stringstream ss;
                    ss << "Native method '" << getMethodName(node) << "' must return an int.";

                    Error(E_WEEDER, token, ss.str());
                }

                if (!oneParameterIntType(node)) {
                    std::stringstream ss;
                    ss << "Native method '" << getMethodName(node) << "' must take exactly one parameter of type int.";

                    Error(E_WEEDER, token, ss.str());
                }
            }
        }
};

#endif
