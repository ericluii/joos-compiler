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
 * (6.6.2.1: If the access is by a field access expression E.Id, where
 * E is a Primary expression, or by a method invocation expression
 * E.Id(. . .), where E is a Primary expression, then the access is
 * permitted if and only if the type of E is S or a subclass of S,
 * where S is the subclass of the declaring class C, in which the
 * access occurs.
*/
public class Main extends p.A {

    public Main() {}

    public int foo(){
	p.A a = this;
	return a.f;
    }

    public static int test() {
	return 123;
    }
}
