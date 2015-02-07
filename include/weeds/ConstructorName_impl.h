#ifndef __WEED_CONSTRUCTOR_NAME_H__
#define __WEED_CONSTRUCTOR_NAME_H__

#include "weed.h"

class ConstructorName : public Weed {
    public:
        ConstructorName() {
            rule = CONSTRUCTOR_PARTS;
        }

        std::string getConstructorName(ParseTree* node) {
            token = node->children[1]->children[0]->children[0]->children[0]->token;
            return token->getString();
        }

        void check(ParseTree* node) {
            std::string constructorName = getConstructorName(node);
            if(constructorName != className) {
                // under the assumption that className has been assigned by ClassFilename weeder
                std::stringstream ss;
                ss << "Constructor '" << constructorName << "' must have the same name as the class it is in.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};


#endif
