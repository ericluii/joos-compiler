// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type of instanceof expression is boolean.
 */
public class Je_6_Assignable_Instanceof_Result {

    public Je_6_Assignable_Instanceof_Result () {}

    public static int test() {
	int i = (new Object() instanceof Object);
        return 123;
    }

}
