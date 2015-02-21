// JOOS1:TYPE_CHECKING,INVALID_INSTANCEOF
// JOOS2:TYPE_CHECKING,INVALID_INSTANCEOF
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Cannot check instanceof on simple types
 */
public class Je_6_Assignable_Instanceof_SimpleType {

    public Je_6_Assignable_Instanceof_SimpleType () {}

    public static int test() {
	boolean b = (7 instanceof Object);
        return 123;
    }

}
