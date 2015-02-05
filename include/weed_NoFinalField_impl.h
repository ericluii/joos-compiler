#ifndef __WEED_NO_FINAL_FIELD_H__
#define __WEED_NO_FINAL_FIELD_H__

#include "weed.h"
#include <cassert>

class NoFinalField : public Weed
{
    public:
        NoFinalField()
        {
            rule = FIELD_DECL;
        }

        int hasFinalMod(ParseTree* node) {
            int found = 0;

            switch (node->rule) {
                case FIELD_DECL:
                case MEMBER_MOD:
                case MEMBER_MOD_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasFinalMod(node->children[i]);
                    }
                    break;
                case MEMBER_MOD_FINAL:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        std::string getFieldName(ParseTree* node) {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == DECL_VARIABLE ||
                    node->children[i]->rule == DECL_VARIABLE_ASSIGN) {
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

        int check(ParseTree* node)
        {
            if (hasFinalMod(node)) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Field declaration '" << getFieldName(node) << "' cannot be declared as final." << std::endl;
                return 1;
            }

            return 0;
        }
};

#endif
