// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type char is not assignable to type short
 */
public class Je_6_Assignable_short_char {

    public Je_6_Assignable_short_char (){}
    
    public static int test() {
	short x = '*';
	return x + 81;
    }
}

