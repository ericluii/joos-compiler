// TYPE_CHECKING
public class J1_ArrayAccess_Cast {
    public J1_ArrayAccess_Cast() {}
    
    public static int test() {
	int[] a = new int[1];
	a[0] = 123;
	return ((int[])a)[0];
    }
}
