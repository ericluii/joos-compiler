// JOOS1:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JOOS2:TYPE_CHECKING,NON_BOOLEAN_CONDITION
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Condition clause in if must have type boolean.
 */
public class Je_6_Assignable_Condition_SimpleType {

    public Je_6_Assignable_Condition_SimpleType () {}

    public static int test() {
	int i = 17;
	if (i) return 123;
        return 7;
    }

}
