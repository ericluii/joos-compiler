// CODE_GENERATION

public class J1_A_StringConstAEQ_ANE {
	public J1_A_StringConstAEQ_ANE() {}
	
	public static int test() {
		String s = "foo";
		boolean b1 = s == "foo";
		boolean b2 = s != "foo";
		if ((b1 || b2) || !(b1 && b2)) {
			return 123;
		}
		return 7;
	}
}
		
