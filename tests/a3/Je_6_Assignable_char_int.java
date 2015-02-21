// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type int is not assignable to type char.
 */

public class Je_6_Assignable_char_int {

    public Je_6_Assignable_char_int(){}

	public static int test() {
		char x = 1;
		return x + 122;
	}
}

