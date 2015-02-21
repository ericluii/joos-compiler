// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/**
 * Name linking: 
 * - The initializer for a non-static field must not contain a read
 * access of a non-static field declared later in the same class.
 * (This includes the field's own initializer).
 */
public class Je_5_ForwardReference_FieldInOwnInitializer_RightSideOfAssignment {

    public int doop = 4;

    public int foo = (doop = foo);

    public Je_5_ForwardReference_FieldInOwnInitializer_RightSideOfAssignment() { }

    public static int test() { 
	return 123; 
    }
}
