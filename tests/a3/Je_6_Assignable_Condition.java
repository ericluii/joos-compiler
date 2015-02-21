// JOOS1:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JOOS2:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Condition clause in if must have type boolean.
 */
public class Je_6_Assignable_Condition {

    public Je_6_Assignable_Condition () {}

    public static int test() {
	Object obj = new Object();
	if (obj=obj) return 123;
        else return 7;
    }

}
