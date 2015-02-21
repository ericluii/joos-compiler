// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 * 
 * The condition (true && false) is constant folded into ABooleanConstExp, x, with x.value == false,
 * but x.getBool() == null. See Util.makeBooleanConstExp().
 */
public class Je_7_Reachability_WhileFalse_ConstantFolding {
	public Je_7_Reachability_WhileFalse_ConstantFolding() {}
	
	public void method() {
		while (true && false) {} 
	}
	
	public static int test() {
		return 123;
	}
}
