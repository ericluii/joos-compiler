// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 * 
 * The condition (false || true) is constant folded into ABooleanConstExp, x, with x.value == true.
 */
public class Je_7_Reachability_WhileTrue_ConstantFolding {
	public Je_7_Reachability_WhileTrue_ConstantFolding() {}
	
	public void method() {
		while (false || true) {}
		return; // unreachable statement
	}
	
	public static int test() {
		return 123;
	}
}
