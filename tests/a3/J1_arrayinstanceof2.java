// TYPE_CHECKING,CODE_GENERATION
public class J1_arrayinstanceof2 {
	public J1_arrayinstanceof2() { }
	public static int test() {
		String[] s = null;
		if (s instanceof Object)
			return 100;
		else
			return 123;
	}
}
