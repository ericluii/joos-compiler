#ifndef __WEED_NO_FIELD_REPEAT_MODIFIERS_H__
#define __WEED_NO_FIELD_REPEAT_MODIFIERS_H__

#include "NoRepeatModifiers_impl.h"

class NoFieldRepeatModifiers : public NoRepeatModifiers {
    public:
        NoFieldRepeatModifiers() {
            rule = FIELD_DECL;
        }

        std::string getFieldName(ParseTree* node) {
            token = node->children[2]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMultipleSameModifier(node->children[0])) {
                std::stringstream ss;
                ss << "Field '" << getFieldName(node) << "' in class '" << className << "' cannot have a repeating modifier.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
