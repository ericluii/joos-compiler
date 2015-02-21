// CODE_GENERATION

public class J1e_A_CastToArray {
	public J1e_A_CastToArray() {}
	
	public static int test() {
		Object o = new Integer(123);
		int[] ia = (int[])o;
		return 123;
	}
}
