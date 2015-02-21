// DISAMBIGUATION
public final class J1_forwardfield2 {
	public J1_forwardfield2(){
		foo=50;
		int x=foo;
		foo = x*2+23; 
	}
	
	public static int test() {
		return new J1_forwardfield2().foo;
	}
	
	protected int foo;	
}
