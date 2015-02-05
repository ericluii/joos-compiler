#ifndef __WEED_CLASS_CONSTRUCTOR_H__
#define __WEED_CLASS_CONSTRUCTOR_H__

#include "weed.h"
#include <cassert>

class ClassConstructor : public Weed
{
    public:
        ClassConstructor()
        {
            rule = CLASS_DECL;
        }

        unsigned int hasConstructorDeclaration(ParseTree* node)
        {
            unsigned int found = 0;

            switch (node->rule) {
                case CLASS_DECL:
                case CLASS_BODY:
                case CLASS_BODY_DECLSTAR:
                case CLASS_BODY_DECL:
                case CLASS_BODY_LIST:
                    for (unsigned int i = 0; i < node->children.size(); i++) {
                        found += hasConstructorDeclaration(node->children[i]);
                    }
                    break;
                case CLASS_CONSTRUCTOR:
                    return 1;
                default:
                    return 0;
            }

            return found;
        }

        std::string getClassName(ParseTree* node)
        {
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
            if (hasConstructorDeclaration(node) == 0) {
                std::stringstream ss;
                ss << "class '" << getClassName(node) << "' must explicitly declare a constructor.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
