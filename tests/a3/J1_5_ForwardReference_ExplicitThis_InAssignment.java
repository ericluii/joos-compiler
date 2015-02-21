// DISAMBIGUATION

public class J1_5_ForwardReference_ExplicitThis_InAssignment {
	public J1_5_ForwardReference_ExplicitThis_InAssignment() {}
	
	public int foo = (foo = this.foo);
	
	public static int test() {
		return 123;
	}
}
