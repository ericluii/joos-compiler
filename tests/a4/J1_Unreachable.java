// REACHABILITY
public class J1_Unreachable {
	
	public J1_Unreachable(){}
	
	public static int test() {
		String foo = "foo";
		String bar = "bar";
		
		for (int i = 100; foo.equals((Object) bar); i=i+1) {
			i = i + 1;
		}
		return 123;
	}
}
