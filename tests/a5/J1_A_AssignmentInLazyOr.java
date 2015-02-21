// CODE_GENERATION
/* CodeGeneration:
 * 
 * Code for lazy-or must preserve semantics.
 */

public class J1_A_AssignmentInLazyOr {
	public J1_A_AssignmentInLazyOr() {}
	
	public int field = 121;
	
	public int sideeffect() {
		field = field + 1;
		return field;
	}
	
	public static int test() {
		J1_A_AssignmentInLazyOr j = new J1_A_AssignmentInLazyOr();
		int a = 122;
		if ( (a == j.sideeffect()) || (a = j.sideeffect()) > 0) {
			return j.sideeffect();
		}
		return 42;
	}
}
