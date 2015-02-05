#include "weeder.h"
#include "NoAbstractFinal_impl.h"
#include "NoBodyAbstractNative_impl.h"
#include "NoFinalStaticAbstractMethod_impl.h"
#include "NoFinalStaticMethod_impl.h"
#include "RestrictedNativeMethod_impl.h"
#include "NoStaticFinalNativeProtectedInterfaceMethod_impl.h"
#include "ClassConstructor_impl.h"
#include "NoFinalField_impl.h"
#include "NonAbstractNonNativeBody_impl.h"
#include "ClassFilename_impl.h"
#include "InterfaceFilename_impl.h"
#include "NoPackagePrivateClassMethod_impl.h"
#include "NoCastExpression_impl.h"
#include "CheckIntegerValue_impl.h"
#include "NoFinalInterface_impl.h"
#include "NoPackagePrivateFields_impl.h"
#include "NoPackagePrivateType_impl.h"

Weeder::Weeder()
{
    // Add 'weeds' that need to be veried to this vector
    weeds.push_back(new NoAbstractFinal());
    weeds.push_back(new NoAbstractNativeBody());
    weeds.push_back(new NoFinalStaticAbstractMethod());
    weeds.push_back(new NoFinalStaticMethod());
    weeds.push_back(new RestrictedNativeMethod());
    weeds.push_back(new ClassConstructor());
    weeds.push_back(new NoFinalField());
    weeds.push_back(new NoStaticFinalNativeProtectedInterfaceMethod());
    weeds.push_back(new NonAbstractNonNativeBody());
    weeds.push_back(new ClassFilename());
    weeds.push_back(new InterfaceFilename());
    weeds.push_back(new NoPackagePrivateClassMethod());
    weeds.push_back(new NoCastExpression());
    weeds.push_back(new CheckIntegerValue());
    weeds.push_back(new NoFinalInterface());
    weeds.push_back(new NoPackagePrivateFields());
    weeds.push_back(new NoPackagePrivateType());
}


int Weeder::weedParseTree(ParseTree* node)
{
    int errors = 0;

    // Try to match against all weeds
    for (unsigned int i = 0; i < weeds.size(); i++) {
        if (weeds[i]->checkRule(node->rule)) {
            errors += weeds[i]->check(node);
        }
    }

    for (unsigned int j = 0; j < node->children.size(); j++) {
        errors += weedParseTree(node->children[j]);
    }

    return errors;
}
