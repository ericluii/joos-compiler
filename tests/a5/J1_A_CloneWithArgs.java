// CODE_GENERATION

public class J1_A_CloneWithArgs {
	public J1_A_CloneWithArgs() {}
	
	public int clone(int a) {
		return a;
	}
	
	public static int test() {
		J1_A_CloneWithArgs j = new J1_A_CloneWithArgs();
		return j.clone(123);
	}
}
