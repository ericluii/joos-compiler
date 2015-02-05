#ifndef __WEED_NO_FINAL_INTERFACE_H__
#define __WEED_NO_FINAL_INTERFACE_H__

#include "weed.h"

class NoFinalInterface : public Weed {
    public:
        NoFinalInterface() {
            rule = INTERFACE_DECL;
        }

        unsigned int hasFinal(ParseTree* node) {
            unsigned int found = 0;
            while(true) {
                switch(node->rule) {
                    case INTERFACE_DECL:
                    case MODIFIERS_MOD:
                        node = node->children[0];
                        break;
                    case MODIFIERS_LIST:
                        for(unsigned int i = 0; i < node->children.size(); i++) {
                            found+= hasFinal(node->children[i]);
                        }
                        
                        return found;
                    case MOD_FINAL:
                        return 1;
                    default:
                        return 0;
                }
            }

            return found;
        }

        std::string getInterfaceName(ParseTree* node) {
            token = node->children[2]->children[0]->token;
            return node->children[2]->children[0]->token->getString();
        }

        void check(ParseTree* node) {
            if(hasFinal(node)) {
                std::stringstream ss;
                ss << "Interface '" << getInterfaceName(node) << "' cannot be declared as final.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
