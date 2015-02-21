// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * - The initializer for a non-static field must not contain a read
 * access of a non-static field declared later in the same class.
 */
public class Je_5_ForwardReference_FieldDeclaredLater_ComplexExp {
    
    public int field1 = 23 + (field2-12);

    public int field2 = 100;

    public Je_5_ForwardReference_FieldDeclaredLater_ComplexExp() { }

    public static int test() { 
	return 123;
    }
}
