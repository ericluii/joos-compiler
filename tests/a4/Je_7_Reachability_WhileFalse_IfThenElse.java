// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN

public class Je_7_Reachability_WhileFalse_IfThenElse {
	public Je_7_Reachability_WhileFalse_IfThenElse() {}
	
	public static int method(boolean b) {
		while (false) if (b) return 42; else return 43;
		return 123;
	}
	
	public static int test() {
		return Je_7_Reachability_WhileFalse_IfThenElse.method(true);
	}
}
