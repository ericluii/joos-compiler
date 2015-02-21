// JOOS1:DISAMBIGUATION,TYPE_CHECKING,FIELD_NOT_FOUND
// JOOS2:DISAMBIGUATION,TYPE_CHECKING,FIELD_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * - Transform every AAmbiguousInvoke node into one of the following
 * compounds: 
 *    ...
 *    * A non-static method invocation (ANonstaticInvoke) whose
 * receiver is a static field access (AStaticFieldLvalue) with a base
 * type resolved from a prefix of the ambiguous child of the invoke
 * node enclosed in zero or more non-static field accesses
 * (ANonstaticFieldLvalue).
 *    * A static method invocation (AStaticInvokeExp) with a base type
 * resolved from the ambiguous child of the invoke node.
 * 
 * TypeChecking:
 * - Check that all fields, methods and constructors that are to be linked
 * as described in the decoration rules are actually present in the 
 * corresponding class or interface.
 *
 * (Static field Je_5_AmbiguousName_SamePackageAndClassName does not
 * exist in class Je_5_AmbiguousName_SamePackageAndClassName) 
 */
package Je_5_AmbiguousName_SamePackageAndClassName;

public class Je_5_AmbiguousName_SamePackageAndClassName {

    public static void bar() {
	System.out.println("bar");
    }

    public Je_5_AmbiguousName_SamePackageAndClassName() {}

    public void test() {
	Je_5_AmbiguousName_SamePackageAndClassName.Je_5_AmbiguousName_SamePackageAndClassName.bar();
    }

}
