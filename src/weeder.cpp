#include "weeder.h"
#include "weed_NoAbstractFinal_impl.h"
#include "weed_NoBodyAbstract_impl.h"
#include "weed_NoFinalStaticAbstractMethod_impl.h"
#include "weed_NoFinalStaticMethod_impl.h"

Weeder::Weeder()
{
    // Add 'weeds' that need to be veried to this vector
    weeds.push_back(new NoAbstractFinal());
    weeds.push_back(new NoAbstractBody());
    weeds.push_back(new NoFinalStaticAbstractMethod());
    weeds.push_back(new NoFinalStaticMethod());
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
