#ifndef __WEED_CLASS_FILENAME_H__
#define __WEED_CLASS_FILENAME_H__

#include "weed.h"
#include <cassert>

class ClassFilename : public Weed
{
    public:
        ClassFilename()
        {
            rule = CLASS_DECL;
        }

        std::string getClassName(ParseTree* node)
        {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
                    token = node->children[i]->children[0]->token;
                    return node->children[i]->children[0]->token->getString();
                }
            }

            assert(false);
        }

        std::string getFileName(ParseTree* node)
        {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
                    return node->children[i]->children[0]->token->getFile();
                }
            }

            assert(false);
        }

        void check(ParseTree* node)
        {
            // set the class name for the weeders
            className = getClassName(node);
            std::string filename = getFileName(node);

            size_t last_slash = filename.find_last_of('/');
            if (std::string::npos != last_slash) {
                filename.erase(0, last_slash + 1);
            }

            size_t last_dot = filename.find_last_of('.');
            if (std::string::npos != last_dot) {
                filename.erase(last_dot);
            }

            if (filename != className) {
                std::stringstream ss;
                ss << "Class named '" << getClassName(node) << "' does not match the filename '" << getFileName(node) << "'.";

                Error(E_WEEDER, token, ss.str());
            }
        }
};

#endif
