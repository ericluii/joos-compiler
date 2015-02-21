// TYPE_CHECKING
public class J1_ArrayInterfaces {
    public J1_ArrayInterfaces() {}

    public static int test() {
	int[] a = new int[3];
	java.lang.Cloneable c = a;
	java.io.Serializable s = a;
	return 123;
    }
}
