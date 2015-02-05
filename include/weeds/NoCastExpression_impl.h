#ifndef __WEED_NO_CAST_EXPRESSION_H__
#define __WEED_NO_CAST_EXPRESSION_H__

class NoCastExpression : public Weed {
    public:
        NoCastExpression() {
            rule = CAST_TO_EXPRESSION;
        }

        Token* getTokenWithLocation(ParseTree* node) {
            if (node->token) {
                return node->token;
            }

            Token* token;
            for (unsigned int i = 0; i < node->children.size(); i++) {
                token = getTokenWithLocation(node->children[i]);
                if (token) {
                    return token;
                }
            }

            assert(false);
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
                        token = getTokenWithLocation(node);
                        return 1;
                }
            }

            return 0;
        }

        void check(ParseTree* node) {
            if(isAnExpression(node)) {
                std::stringstream ss;
                ss << "Casting to an expression is not allowed.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
