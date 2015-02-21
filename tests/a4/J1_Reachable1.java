// REACHABILITY
public class J1_Reachable1 {
	public J1_Reachable1(){}
	
	public String test2() {
		for (int i = 100; i < 50; i=i+1) {
			i = i + 1;
		}
		return "";
	}
	
	public static int test() {
		return 123;
	}
}
