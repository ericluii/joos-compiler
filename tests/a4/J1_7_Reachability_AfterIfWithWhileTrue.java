// REACHABILITY
public class J1_7_Reachability_AfterIfWithWhileTrue {

	public J1_7_Reachability_AfterIfWithWhileTrue () {}

	public static int test() {
		int x=123;
		if (x==122) while (true);
		return x;
	}
}
