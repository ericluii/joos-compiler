// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type int is not assignable to type short
 */
public class Je_6_Assignable_short_int {

    public Je_6_Assignable_short_int(){}
    
    public static int test() {
	short x = 1;
	return x + 122;
    }
}

