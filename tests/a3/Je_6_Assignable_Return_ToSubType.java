// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type Object is not assignable to type String
 */
public class Je_6_Assignable_Return_ToSubType {

    public Je_6_Assignable_Return_ToSubType () {}

    public String m() {
	return new Object();
    }

    public static int test() {
        return 123;
    }

}
