// JOOS1:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JOOS2:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - While condition must be of type boolean
 */
public class Je_6_Assignable_Condition_While {

    public Je_6_Assignable_Condition_While () {}

    public static int test() {
	while (new Object()) {
	    int i = 0;
	}
	return 123;
    }

}
