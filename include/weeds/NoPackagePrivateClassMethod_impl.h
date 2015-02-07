#ifndef __WEED_NO_PACKAGE_PRIVATE_CLASS_METHOD_H__
#define __WEED_NO_PACKAGE_PRIVATE_CLASS_METHOD_H__

#include "weed.h"
#include <cassert>

class NoPackagePrivateClassMethod : public Weed {
    public:
        NoPackagePrivateClassMethod() {
            rule = METHOD_HEADER_AND_BODY;
        }

        unsigned int hasMod(unsigned int rule, ParseTree* node) {
            int found = 0;

            switch(node->rule) {
                case METHOD_HEADER_AND_BODY:
                case METHOD_TYPE:
                case METHOD_VOID:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for(unsigned int i = 0; i < node->children.size(); i++) {
                        found+= hasMod(rule, node->children[i]);
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

        void check(ParseTree* node) {
            unsigned int hasPublic = hasMod(MEMBER_MOD_PUBLIC, node);
            unsigned int hasProtected = hasMod(MEMBER_MOD_PROTECTED, node);
            
            if (!hasPublic && !hasProtected) {
                std::stringstream ss;
                ss << "Method '" << getMethodName(node) << "' in class cannot be a package private method.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
