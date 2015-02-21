// DISAMBIGUATION,CODE_GENERATION
public class J1_backwardRef {
	public int a = 2;
	public int b = a + 121;
	
	public J1_backwardRef() {
	}
	
	public static int test() {
		return new J1_backwardRef().b;
	}
}
