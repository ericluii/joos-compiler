// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 *  Typecheck:
 *  - Type byte is not assignable to type char
 */

public class Je_6_Assignable_char_byte_1 {

    public Je_6_Assignable_char_byte_1(){}

	public static int test() {
		char x = (byte)123456;
		return x + 59;
	}
}

