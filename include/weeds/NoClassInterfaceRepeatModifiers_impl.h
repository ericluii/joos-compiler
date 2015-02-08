#ifndef __WEED_NO_CLASS_INTERFACE_REPEAT_MODIFIERS_H__
#define __WEED_NO_CLASS_INTERFACE_REPEAT_MODIFIERS_H__

#include "NoRepeatModifiers_impl.h"

class NoClassInterfaceRepeatModifiers : public NoRepeatModifiers {
    private:
        int ruleSecond;
    public:
        NoClassInterfaceRepeatModifiers() {
            rule = CLASS_DECL;
            ruleSecond = INTERFACE_DECL;
        }

        bool checkRule(int check) {
            return (check == rule || check == ruleSecond);
        }

        std::string getObjectName(ParseTree* node) {
            token = node->children[2]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            if(hasMultipleSameModifier(node->children[0])) {
                std::stringstream ss;
                if(node->rule == rule) {
                    ss << "Class '";
                } else {
                    ss << "Interface '";
                }
                
                ss << getObjectName(node) << "' cannot have a repeating modifier.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
