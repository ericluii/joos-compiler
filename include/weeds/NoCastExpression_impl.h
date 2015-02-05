#ifndef __WEED_NO_CAST_EXPRESSION_H__
#define __WEED_NO_CAST_EXPRESSION_H__

class NoCastExpression : public Weed {
    public:
        NoCastExpression() {
            rule = CAST_TO_EXPRESSION;
        }

        unsigned int isAnExpression(ParseTree* node) {
            while(true) {
                switch(node->rule) {
                    case CAST_TO_EXPRESSION:
                        node = node->children[1];
                        break;
                    case EXPRESSION_COND:
                    case COND_TO_CONDOR:
                    case CONDOR_TO_CONDAND:
                    case CONDAND_TO_INCLUOR:
                    case INCLUOR_TO_AND:
                    case AND_TO_EQUALITY:
                    case EQUALITY_TO_RELATIONAL:
                    case RELATION_TO_ADDITIVE:
                    case ADD_TO_MULTI:
                    case MULTI_TO_UNARY:
                    case NOT_NEG_UNARY:
                        node = node->children[0];
                        break;
                    case UNARY_NAME:
                        return 0;
                    default:
                        return 1;
                }
            }

            return 0;
        }

        unsigned int check(ParseTree* node) {
            if(isAnExpression(node)) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Casting to an expression is not allowed" << std::endl;
                return 1;
            }
            
            return 0;
        }
};

#endif
