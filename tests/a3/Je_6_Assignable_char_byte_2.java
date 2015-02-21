// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type byte is not assignable to type char
 */

public class Je_6_Assignable_char_byte_2 {

    public Je_6_Assignable_char_byte_2(){}

	public static int test() {
		char x = (byte)1;
		return x + 122;
	}
}

