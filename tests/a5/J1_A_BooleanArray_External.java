// CODE_GENERATION
/* CodeGeneration:
 * 
 * byte array are used for boolean array in JVM
 */

public class J1_A_BooleanArray_External {
	public J1_A_BooleanArray_External() {}
	
	public static int test() {
		boolean[] a = new boolean[2];
		a[0] = true;
		a[1] = a[0];
		if (java.util.Arrays.equals(a,a)) return 123;
		return 42;
	}
}
