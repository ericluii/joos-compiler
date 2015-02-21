// JOOS1:TYPE_CHECKING,NONSTATIC_FIELD_LINKED_AS_STATIC
// JOOS2:TYPE_CHECKING,NONSTATIC_FIELD_LINKED_AS_STATIC
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that fields and methods linked as static are actually
 * static, and that those linked as non-static are actually
 * non-static.
 */
public class Je_6_StaticAccessToNontatic_Field {

    public int field = 23;

    public Je_6_StaticAccessToNontatic_Field() { }

    public static int test() { 
	return Je_6_StaticAccessToNontatic_Field.field+100;
    }
}
