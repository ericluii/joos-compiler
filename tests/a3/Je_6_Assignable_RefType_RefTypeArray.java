// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type A[] is not assignable to type A.
 */
public class Je_6_Assignable_RefType_RefTypeArray {

    public Je_6_Assignable_RefType_RefTypeArray () {}

    public static int test() {
	Integer j = new Integer[10];
	return 123;
    }

}
