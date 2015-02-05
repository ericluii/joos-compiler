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

        unsigned int hasFinalMod(ParseTree* node) {
            unsigned int found = 0;

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
                    token = node->children[i]->children[0]->token;
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
        }

        void check(ParseTree* node)
        {
            if (hasFinalMod(node)) {
                std::stringstream ss;
                ss << "Field declaration '" << getFieldName(node) << "' cannot be declared as final.";
                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
