#ifndef __WEED_NO_CLASS_METHOD_REPEAT_MODIFIERS_H__
#define __WEED_NO_CLASS_METHOD_REPEAT_MODIFIERS_H__

#include "NoRepeatModifiers_impl.h"

class NoClassMethodRepeatModifiers : public NoRepeatModifiers {
    private:
        int ruleSecond;
    public:
        NoClassMethodRepeatModifiers() {
            rule = METHOD_TYPE;
            ruleSecond = METHOD_VOID;
        }

        bool checkRule(int check) {
            return (check == rule || check == ruleSecond);
        }

        std::string getClassMethodName(ParseTree* node) {
            token = node->children[2]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMultipleSameModifier(node->children[0])) {
                std::stringstream ss;
                ss << "Method '" << getClassMethodName(node) << "' in class '" << className << "' cannot have a repeating modifier.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
