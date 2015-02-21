// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/* Disambiguation:
 * The initializer for a non-static field must not refer by simple name to
 * itself or a non-static field declared later in the same class, except as the
 * direct left-hand side of an assignment.
 */
public class Je_5_ForwardReference_MethodCall {
    public Je_5_ForwardReference_MethodCall() {}

    public int y = z.method(); // this is an illegal forward reference
    public Je_5_ForwardReference_MethodCall z = new Je_5_ForwardReference_MethodCall();
    
    public int method() {
	return 123;
    }
    
    public static int test() {
	return 123;
    }
}
