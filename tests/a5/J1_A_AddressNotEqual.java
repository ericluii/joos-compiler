// CODE_GENERATION
/* CodeGeneration:
 * 
 * Code generation must preserve semantics.
 */

public class J1_A_AddressNotEqual {
	public J1_A_AddressNotEqual() {}
	
	public static int test() {
		Object o = new J1_A_AddressNotEqual();
		if (o != null) {
			return 123;
		}
		return 42;
	}
}
