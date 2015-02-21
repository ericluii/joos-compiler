// REACHABILITY

public class J1_7_Reachability_IfThenElse_InValueMethod {
	public J1_7_Reachability_IfThenElse_InValueMethod() {}
	
	public int method(boolean b) {
		if (b) return 42;
		else return 123;
	}
	
	public static int test() {
		return 123;
	}
}
