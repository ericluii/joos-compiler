// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type int[] is not assignable to type byte[]
 */
public class Je_6_Assignable_byteArray_intArray {

    public Je_6_Assignable_byteArray_intArray(){}
    
    public static int test(){
	byte[] a = new int[7];
	return 123;
    }
}

