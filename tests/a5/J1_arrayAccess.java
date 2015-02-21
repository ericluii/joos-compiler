// TYPE_CHECKING,CODE_GENERATION
public class J1_arrayAccess {

    public J1_arrayAccess() {}

    public static int test() {
	int[] a = new int[4];
	a[0] = 1;
	a[1] = 12;
	a[2] = 123;
	a[3] = 1234;
	return a[2];
    }
}
