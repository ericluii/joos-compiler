// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/**
 * Name linking: 
 * - The initializer for a non-static field must not contain a read
 * access of a non-static field declared later in the same class.
 */
public class Je_5_ForwardReference_FieldDeclaredLater {

    public Je_5_ForwardReference_FieldDeclaredLater () {}

    public int i = j;
    public int j = 1;

    public static int test() {
        return 7;
    }

}
