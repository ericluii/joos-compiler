#ifndef __WEED_CHECK_INTEGER_VALUE_H__
#define __WEED_CHECK_INTEGER_VALUE_H__

#include "weed.h"
#include <sstream>

class CheckIntegerValue : public Weed {
    private:
        std::string integerVal;
        int ruleSecond = MULTI_TO_MULTUNARY;
        int ruleThird = MULTI_TO_DIVUNARY;
        int ruleFourth = MULTI_TO_MODUNARY;
    public:
        CheckIntegerValue() {
            rule = MULTI_TO_UNARY;
        }

        bool checkRule(int check) {
            return (rule == check || ruleSecond == check || ruleThird == check || ruleFourth == check);
        }

        unsigned int isTooSmallOrTooBig(ParseTree* node) {
            std::string prefix = "";
            std::stringstream ss;
            int numVal;
            while(true) {
                switch(node->rule) {
                    case MULTI_TO_MULTUNARY:
                    case MULTI_TO_DIVUNARY:
                    case MULTI_TO_MODUNARY:
                        node = node->children[2];
                        break;
                    case MULTI_TO_UNARY:
                        node = node->children[0];
                        break;
                    case NEG_UNARY:
                        prefix = "-";
                        node = node->children[1];
                        break;
                    case NOT_NEG_UNARY:
                    case PRIMARY_UNARY:
                    case PRIMARY_NONARRAY:
                    case PRIMARY_LITERAL:
                        node = node->children[0];
                        break;
                    case LITERAL_NUM:
                        integerVal = prefix+node->children[0]->token->getString();
                        ss << integerVal;
                        if(!(ss >> numVal)) {
                            return 1;
                        }
                    default:
                        return 0;
                }
            }

            return 0;
        }

        unsigned int check(ParseTree* node) {
            if (isTooSmallOrTooBig(node)) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Integer: " << integerVal << " is out of range. "
                          << "Should be between -2147483648 and 214748367, inclusive" << std::endl;
                return 1;
            }

            return 0;

        }
};


#endif
