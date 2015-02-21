// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type char is not assignable to type byte
 */

public class Je_6_Assignable_byte_char {

    public Je_6_Assignable_byte_char(){}

	public static int test() {
		byte x = '*';
		return x + 81;
	}
}

