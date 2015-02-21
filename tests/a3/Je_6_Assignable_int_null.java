// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Null type not assignable to type int
 */
public class Je_6_Assignable_int_null {

    public Je_6_Assignable_int_null () {}

    public static int test() {
        int i = null;
	return 123;
    }

}
