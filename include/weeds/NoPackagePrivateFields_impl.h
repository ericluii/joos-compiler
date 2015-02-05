#ifndef __WEED_NO_PACKAGE_PRIVATE_FIELDS_H__
#define __WEED_NO_PACKAGE_PRIVATE_FIELDS_H__

#include "weed.h"

class NoPackagePrivateFields : public Weed {
    public:
        NoPackagePrivateFields() {
            rule = FIELD_DECL;
        }

        unsigned int hasMod(unsigned int rule, ParseTree* node) {
            int found = 0;

            switch(node->rule) {
                case FIELD_DECL:
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

        std::string getFieldName(ParseTree* node) {
            return node->children[2]->children[0]->children[0]->token->getString();
        }

        unsigned int check(ParseTree* node) {
            unsigned int hasPublic = hasMod(MEMBER_MOD_PUBLIC, node);
            unsigned int hasProtected = hasMod(MEMBER_MOD_PROTECTED, node);

            if(hasPublic && hasProtected) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Field '" << getFieldName(node) << "' in class cannot be both public and protected." << std::endl;
                return 1;
            }

            if (!hasPublic && !hasProtected) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Field '" << getFieldName(node) << "' in class cannot be a package private field." << std::endl;
                return 1;
            }

            return 0;
        }
};

#endif
