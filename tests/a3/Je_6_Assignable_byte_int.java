// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type int is not assignable to type byte.
 */

public class Je_6_Assignable_byte_int {

    public Je_6_Assignable_byte_int(){}

	public static int test() {
		byte x = 1;
		return x + 122;
	}
}

