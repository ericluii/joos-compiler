#include "weeder.h"
#include "weed_NoAbstractFinal_impl.h"

Weeder::Weeder()
{
    // Add 'weeds' that need to be veried to this vector
    weeds.push_back(new NoAbstractFinal());
}


int Weeder::weedParseTree(ParseTree* node)
{
    int errors = 0;

    // Try to match against all weeds
    for (unsigned int i = 0; i < weeds.size(); i++) {
        if (weeds[i]->rule == node->rule) {
            errors += weeds[i]->check(node);
        }
    }

    for (unsigned int j = 0; j < node->children.size(); j++) {
        errors += weedParseTree(node->children[j]);
    }

    return errors;
}
