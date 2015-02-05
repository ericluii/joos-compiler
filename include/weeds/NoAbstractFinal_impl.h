#ifndef __WEEDS_NO_ABSTRACT_FINAL_H__
#define __WEEDS_NO_ABSTRACT_FINAL_H__

#include "weed.h"
#include <cassert>

class NoAbstractFinal : public Weed
{
    public:
        NoAbstractFinal()
        {
            rule = CLASS_DECL;
        }

        unsigned int hasAbstractMod(ParseTree* node) {
            unsigned int found = 0;

            switch (node->rule) {
                case CLASS_DECL:
                case MODIFIERS_MOD:
                case MODIFIERS_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasAbstractMod(node->children[i]);
                    }
                    break;
                case MOD_ABSTRACT:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        unsigned int hasFinalMod(ParseTree* node) {
            unsigned int found = 0;

            switch (node->rule) {
                case CLASS_DECL:
                case MODIFIERS_MOD:
                case MODIFIERS_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasFinalMod(node->children[i]);
                    }
                    break;
                case MOD_FINAL:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        std::string getClassName(ParseTree* node) {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
            return "COMPILER ERROR - NOTIFY CREATORS";
        }

        unsigned int check(ParseTree* node)
        {
            if (hasAbstractMod(node) && hasFinalMod(node)) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "class '" << getClassName(node) << "' cannot be declared as both final and abstract." << std::endl;
                return 1;
            }

            return 0;
        }
};

#endif
