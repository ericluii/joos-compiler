// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 */
public class Je_7_Reachability_AfterReturnEmptyBlock {

	public Je_7_Reachability_AfterReturnEmptyBlock () {}

	public static int test(int j) {
		return 123;
		{}
	}

}
