// DISAMBIGUATION
public final class J1_forwardfield1 {
	public J1_forwardfield1(){
		foo = (true || false) && !(true || false);
		System.out.println(foo);
	}
	
	public static int test() {
		return 123;
	}
	
	protected boolean foo;	
}
