// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND,PROTECTED_CONSTRUCTOR_INVOCATION
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND,PROTECTED_CONSTRUCTOR_INVOCATION
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all accesses to protected fields, methods and
 * constructors occur from within the same package as, or from within
 * a subtype of, the class or interface declaring the accessed
 * entity. A class instance creation expression invoking a protected
 * constructor must be in the same package as the created class.
 */
import p.A;

public class Main {

    public Main () {}

    public static int test() {
	Object o = new A();
        return 123;
    }

}
