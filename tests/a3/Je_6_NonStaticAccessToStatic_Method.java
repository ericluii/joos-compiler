// JOOS1:TYPE_CHECKING,STATIC_METHOD_LINKED_AS_NONSTATIC
// JOOS2:TYPE_CHECKING,STATIC_METHOD_LINKED_AS_NONSTATIC
// JAVAC:
/**
 * Typecheck:
 * - Check that fields and methods linked as static are actually
 * static, and that those linked as non-static are actually
 * non-static.
 */
public class Je_6_NonStaticAccessToStatic_Method {

    public Je_6_NonStaticAccessToStatic_Method() {}

    public static int test() {
	Integer integer = new Integer(42);
	return integer.parseInt("123");
    }
    
}
