// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * - The initializer for a non-static field must not refer by simple
 * name to itself or a non-static field declared later in the same
 * class, except as the direct left-hand side of an assignment.
 */
public class Je_5_ForwardReference_FieldInOwnInitializer_ReadAfterAssignment {
    
    public int a = (a = 2) + a;
    
    public Je_5_ForwardReference_FieldInOwnInitializer_ReadAfterAssignment() {
    }
    
    public static int test() {
	return 123;
    }
}
