#ifndef __WEED_NO_CONSTRUCTOR_PUBLIC_AND_PROTECTED_H__
#define __WEED_NO_CONSTRUCTOR_PUBLIC_AND_PROTECTED_H__

#include "weed.h"
#include "NoPublicAndProtected_impl.h"

class NoConstructorPublicAndProtected : public NoPublicAndProtected {
    public:
        NoConstructorPublicAndProtected() {
            rule = CONSTRUCTOR_PARTS;
        }

        std::string getConstructorName(ParseTree* node) {
            token = node->children[1]->children[0]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMod(MEMBER_MOD_PUBLIC, node) && hasMod(MEMBER_MOD_PROTECTED, node)) {
                std::stringstream ss;
                ss << "Constructor '" << getConstructorName(node) << "' in class cannot be both public and protected.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
