#ifndef __WEED_NO_CONSTRUCTOR_REPEAT_MODIFIERS_H__
#define __WEED_NO_CONSTRUCTOR_REPEAT_MODIFIERS_H__

#include "NoRepeatModifiers_impl.h"

class NoConstructorRepeatModifiers : public NoRepeatModifiers {
    public:
        NoConstructorRepeatModifiers() {
            rule = CONSTRUCTOR_PARTS;
        }

        std::string getConstructorName(ParseTree* node) {
            token = node->children[1]->children[0]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMultipleSameModifier(node->children[0])) {
                std::stringstream ss;
                ss << "Constructor '" << getConstructorName(node) << "' in class '" << className << "' cannot have a repeating modifier.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
