// JOOS1:TYPE_CHECKING,STATIC_FIELD_LINKED_AS_NONSTATIC
// JOOS2:TYPE_CHECKING,STATIC_FIELD_LINKED_AS_NONSTATIC
// JAVAC:
/**
 * Typecheck:
 * - Check that fields and methods linked as static are actually
 * static, and that those linked as non-static are actually
 * non-static.
 */
public class Je_6_NonStaticAccessToStatic_Field {
    
    public Je_6_NonStaticAccessToStatic_Field() {}
    
    public static int test() {	
	Integer integer = new Integer(2147483524);
	return integer.MAX_VALUE-integer.intValue();
    }
    
}
