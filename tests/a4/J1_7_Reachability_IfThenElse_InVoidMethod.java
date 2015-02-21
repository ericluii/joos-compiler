// REACHABILITY

public class J1_7_Reachability_IfThenElse_InVoidMethod {
	public J1_7_Reachability_IfThenElse_InVoidMethod() {}
	
	public void method(boolean b) {
		if (b) return;
		else return;
	}
	
	public static int test() {
		return 123;
	}
}
