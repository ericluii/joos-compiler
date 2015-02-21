// REACHABILITY
/**
 * Reachability:
 * - If the body of a method whose return type is not void can
 * complete normally, produce an error message.  
 *
 * The condition (false || true) is constant folded into ABooleanConstExp, x, with x.value == true.
 */
public class J1_7_Reachability_WhileTrue_ConstantFolding {
	public J1_7_Reachability_WhileTrue_ConstantFolding() {}
	
	public int method() {
		while (false || true) {}
	}
	
	public static int test() {
		return 123;
	}
}
