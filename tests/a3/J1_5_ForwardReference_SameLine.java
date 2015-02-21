//DISAMBIGUATION

public class J1_5_ForwardReference_SameLine {
	public int a = 0; public int b = a;
	
	public J1_5_ForwardReference_SameLine() {}
	
	public static int test() {
		return 123;
	}
}
