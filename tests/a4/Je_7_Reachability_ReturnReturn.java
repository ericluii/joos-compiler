// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.
 */
public class Je_7_Reachability_ReturnReturn {

    public Je_7_Reachability_ReturnReturn(){}

    public static int test() {
	return 123;
	return 42;
    }
    
}
