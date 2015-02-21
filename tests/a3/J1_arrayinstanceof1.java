// TYPE_CHECKING,CODE_GENERATION
public class J1_arrayinstanceof1 {
	public J1_arrayinstanceof1() { }
	public static int test() {
		String[] s=new String[0];
		if (s instanceof Object)
			return 123;
		else
			return 100;
	}
}
