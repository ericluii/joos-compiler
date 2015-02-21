// REACHABILITY
public class J1_Reachable4 {
	public J1_Reachable4(){}
	
	public static int test() {
		int x = 1;
		if (false) { x=3; }
		return 122 + x;
	}
}
