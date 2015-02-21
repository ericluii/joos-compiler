// REACHABILITY
public class J1_7_Reachability_IfAndWhile_Return {
	public J1_7_Reachability_IfAndWhile_Return() {}
	public static int test() {
		int x=121;
		if (x==120)	return x;
		while (x==121) return x+2;
		return x;
	}
}
