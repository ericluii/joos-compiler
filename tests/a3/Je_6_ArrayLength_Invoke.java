// JOOS1:TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JOOS2:TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Invocations on simple types not allowed.
 */
public class Je_6_ArrayLength_Invoke{

    public Je_6_ArrayLength_Invoke(){}

    public static int test(){
	int[] a = new int[42];
	return a.length.toString().length();
    }

}
