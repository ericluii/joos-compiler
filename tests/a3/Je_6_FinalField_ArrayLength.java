// JOOS1:TYPE_CHECKING,ASSIGN_TO_ARRAY_LENGTH
// JOOS2:TYPE_CHECKING,ASSIGN_TO_ARRAY_LENGTH
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - A final field must not be assigned to. (Array.length is final)
 */
public class Je_6_FinalField_ArrayLength {
    
    public Je_6_FinalField_ArrayLength () {}
    
    public static int test() {
	int[] a = new int[7];
	a.length = 9;
        return 123;
    }

}
