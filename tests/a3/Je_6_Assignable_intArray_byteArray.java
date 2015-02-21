// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type byte[] is not assignable to type int[]
 */
public class Je_6_Assignable_intArray_byteArray {

    public Je_6_Assignable_intArray_byteArray(){}
    
    public static int test(){
	int[] a = new byte[7];
	return 123;
    }
}

