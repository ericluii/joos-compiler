// DISAMBIGUATION

public class J1_5_ForwardReference_EqualInfix {
	
	public J1_5_ForwardReference_EqualInfix a = null;
	
	public int b = a.c.b;
	
	public J1_5_ForwardReference_EqualInfix c = null;
	
	public J1_5_ForwardReference_EqualInfix() {}
	
	public static int test() {
		return 123;
	}
}
