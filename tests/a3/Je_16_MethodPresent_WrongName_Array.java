// JOOS1: TYPE_CHECKING,JOOS1_ARRAY_METHOD_CALL
// JOOS2: TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
/**
 * Typecheck: 
 * - Check that all fields, methods and constructors that
 *   are to be linked as described in the decoration rules are actually
 *   present in the corresponding class or interface.  
 */
public class Je_16_MethodPresent_WrongName_Array {
	
    public Je_16_MethodPresent_WrongName_Array() {}
    
    public static int test() {
	int[] s = new int[2];
	s.toStringzz();   //Non-existing method on array type
	
	return 123;
    }
}
