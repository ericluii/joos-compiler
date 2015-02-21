// JOOS1:HIERARCHY,TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:HIERARCHY,TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type Object is not assignable to type String
 */
public class Je_6_Assignable_ResultTypeOfAssignment {

    public Je_6_Assignable_ResultTypeOfAssignment () {}

    public static int test() {
	Object o = null;
	String j = (o = "This is not a message");
	return 123;
    }

}
