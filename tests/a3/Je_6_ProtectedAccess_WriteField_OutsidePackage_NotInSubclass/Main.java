// JOOS1:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all accesses to protected fields, methods and
 * constructors occur from within the same package as, or from within
 * a subtype of, the class or interface declaring the accessed
 * entity. A class instance creation expression invoking a protected
 * constructor must be in the same package as the created class.
 * (6.6.2.1: Let C be the class in which a protected member m is
 * declared. Access is permitted only within the body of a subclass S
 * of C).
 */
import Baz.Foo;

public class Main {

    public Main() {
	Foo f = new Foo();
	f.bar = 3;
    }
    
    public static int test() {
	return 123;
    }
}
