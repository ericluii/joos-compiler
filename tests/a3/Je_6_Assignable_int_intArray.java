// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type int[] is not assignable to type int
 */
public class Je_6_Assignable_int_intArray {

    public Je_6_Assignable_int_intArray () {}

    public static int test() {
        int i = new int[10];
	return 123;
    }

}
