#ifndef __WEED_INTERFACE_FILENAME_H__
#define __WEED_INTERFACE_FILENAME_H__

#include "weed.h"
#include <cassert>

class InterfaceFilename : public Weed
{
    public:
        InterfaceFilename()
        {
            rule = INTERFACE_DECL;
        }

        std::string getInterfaceName(ParseTree* node)
        {
            for (unsigned int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->rule == IDENTIFIER) {
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

        unsigned int check(ParseTree* node)
        {
            std::string interfacename = getInterfaceName(node);
            std::string filename = getFileName(node);

            size_t last_slash = filename.find_last_of('/');
            if (std::string::npos != last_slash) {
                filename.erase(0, last_slash + 1);
            }

            size_t last_dot = filename.find_last_of('.');
            if (std::string::npos != last_dot) {
                filename.erase(last_dot);
            }

            if (filename != interfacename) {
                std::cerr << "Weeding error in file: TODO" << std::endl;
                std::cerr << "Interface named '" << getInterfaceName(node) << "' does not match the filename '" << getFileName(node) << "'." << std::endl;
                return 1;
            }

            return 0;
        }
};

#endif
