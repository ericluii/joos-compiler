#ifndef __WEED_NO_PACKAGE_PRIVATE_TYPE_H__
#define __WEED_NO_PACKAGE_PRIVATE_TYPE_H__

#include "weed.h"

class NoPackagePrivateType : public Weed {
    private:
        int rule2;
    public:
        NoPackagePrivateType() {
            rule = CLASS_DECL;
            rule2 = INTERFACE_DECL;
        }

        bool checkRule(int check) {
            return (check == rule || check == rule2);
        }

        unsigned int hasPublicMod(ParseTree* node) {
            unsigned int found = 0;
            while(true) {
                switch(node->rule) {
                    case INTERFACE_DECL:
                    case CLASS_DECL:
                    case MODIFIERS_MOD:
                        node = node->children[0];
                        break;
                    case MODIFIERS_LIST:
                        for(unsigned int i = 0; i < node->children.size(); i++) {
                            found+= hasPublicMod(node->children[i]);
                        }

                        return found;
                    case MOD_PUBLIC:
                        return 1;
                    default:
                        return 0;
                }
            }
            return found;
        }

        std::string getObjectName(ParseTree* node) {
            token = node->children[2]->children[0]->token;
            return node->children[2]->children[0]->token->getString();
        }

        void check(ParseTree* node) {
            if(!hasPublicMod(node)) {
                std::stringstream ss;

                if(node->rule == rule) {
                    ss << "Class '";
                } else {
                    ss << "Interface '";
                }
                ss << getObjectName(node) << "' must be public.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
