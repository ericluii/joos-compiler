#ifndef __WEED_PUBLIC_OR_PROTECTED_CONSTRUCTOR_H__
#define __WEED_PUBLIC_OR_PROTECTED_CONSTRUCTOR_H__

#include "weed.h"

class PublicOrProtectedConstructor : public Weed {
    public:
        PublicOrProtectedConstructor() {
            rule = CONSTRUCTOR_PARTS;
        }

        unsigned int hasNonPublicProtected(ParseTree* node) {
            unsigned int found = 0;

            switch(node->rule) {
                case CONSTRUCTOR_PARTS:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for(unsigned int i = 0; i < node->children.size(); i++) {
                        found+= hasNonPublicProtected(node->children[i]);
                    }
                    break;
                case MEMBER_MOD_ABSTRACT:
                case MEMBER_MOD_STATIC:
                case MEMBER_MOD_NATIVE:
                case MEMBER_MOD_FINAL:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        std::string getConstructorName(ParseTree* node) {
            token = node->children[1]->children[0]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasNonPublicProtected(node)) {
                std::stringstream ss;
                ss << "Constructor '" << getConstructorName(node) << "' must only be either protected or public.";
                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
