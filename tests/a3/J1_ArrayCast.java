// TYPE_CHECKING
public class J1_ArrayCast {
	public J1_ArrayCast() { }
	public static int test() {
		int[] foo = new int[42];
		Object bar=null;
		bar = (int[])foo;
		return 123;
	}
}
