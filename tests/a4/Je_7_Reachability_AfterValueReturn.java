// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 */
public class Je_7_Reachability_AfterValueReturn {

    public Je_7_Reachability_AfterValueReturn () {}
    
    public static int test() {
	return 7;
	int i = 8;
	return 123;
    }

}
