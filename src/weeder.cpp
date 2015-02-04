#include "weeder.h"
#include "weed_NoAbstractFinal_impl.h"
#include "weed_NoBodyAbstractNative_impl.h"
#include "weed_NoFinalStaticAbstractMethod_impl.h"
#include "weed_NoFinalStaticMethod_impl.h"
#include "weed_NativeAndStatic_impl.h"
#include "weed_NoStaticFinalNativeInterfaceMethod_impl.h"
#include "weed_ClassConstructor_impl.h"
#include "weed_NoFinalField_impl.h"
#include "weed_NonAbstractNonNativeBody_impl.h"
#include "weed_ClassFilename_impl.h"
#include "weed_InterfaceFilename_impl.h"

Weeder::Weeder()
{
    // Add 'weeds' that need to be veried to this vector
    weeds.push_back(new NoAbstractFinal());
    weeds.push_back(new NoAbstractNativeBody());
    weeds.push_back(new NoFinalStaticAbstractMethod());
    weeds.push_back(new NoFinalStaticMethod());
    weeds.push_back(new NativeAndStaticMethod());
    weeds.push_back(new ClassConstructor());
    weeds.push_back(new NoFinalField());
    weeds.push_back(new NoStaticFinalNativeInterfaceMethod());
    weeds.push_back(new NonAbstractNonNativeBody());
    weeds.push_back(new ClassFilename());
    weeds.push_back(new InterfaceFilename());
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
