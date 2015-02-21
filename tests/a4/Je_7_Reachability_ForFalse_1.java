// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 */
public class Je_7_Reachability_ForFalse_1 {
    
    public Je_7_Reachability_ForFalse_1(){}
    
    public String test() {
	for (int i = 100; 3 == (5+3*10); i=i+1) {
	    i = i + 1;
	}
	return "";
    }

}
