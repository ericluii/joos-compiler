#ifndef __WEED_NO_METHOD_PUBLIC_AND_PROTECTED_H__
#define __WEED_NO_METHOD_PUBLIC_AND_PROTECTED_H__

#include "weed.h"
#include "NoPublicAndProtected_impl.h"

class NoMethodPublicAndProtected : public NoPublicAndProtected {
    public:
        NoMethodPublicAndProtected() {
            rule = METHOD_HEADER_AND_BODY;
        }

        std::string getMethodName(ParseTree* node) {
            token = node->children[0]->children[2]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMod(MEMBER_MOD_PUBLIC, node) && hasMod(MEMBER_MOD_PROTECTED, node)) {
                std::stringstream ss;
                ss << "Method '" << getMethodName(node) << "' in class cannot be both public and protected.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
