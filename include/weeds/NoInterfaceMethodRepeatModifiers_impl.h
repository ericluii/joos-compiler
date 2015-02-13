#ifndef __WEED_NO_INTERFACE_METHOD_REPEAT_MODIFIERS_H__
#define __WEED_NO_INTERFACE_METHOD_REPEAT_MODIFIERS_H__

#include "NoRepeatModifiers_impl.h"

class NoInterfaceMethodRepeatModifiers : public NoRepeatModifiers {
    private:
        int ruleSecond;
    public:
        NoInterfaceMethodRepeatModifiers() {
            rule = ABSTRACT_METHOD_TYPE;
            ruleSecond = ABSTRACT_METHOD_VOID;
        }

        bool checkRule(int check) {
            return (check == rule || check == ruleSecond);
        }

        std::string getInterfaceMethodName(ParseTree* node) {
            token = node->children[2]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMultipleSameModifier(node->children[0])) {
                std::stringstream ss;
                ss << "Interface method '" << getInterfaceMethodName(node) << "' in interface cannot have a repeating modifier.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
