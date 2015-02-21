// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 */
public class Je_7_Reachability_ForFalse_2 {

    public Je_7_Reachability_ForFalse_2(){}

    public static int test(){
	for (int i = 0; false; i=i+1) {
	    i = i + 1;
	}
	return 123;
    }
}

