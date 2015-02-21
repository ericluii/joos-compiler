// DISAMBIGUATION
/** Tests long static dot sequences
 */

public class J1_namelinking3 {
    public J1_namelinking3() {}
    public static int test() {
	System.out.println("foo");
	java.lang.System.out.println("foo");
	return 123;
    }
}
