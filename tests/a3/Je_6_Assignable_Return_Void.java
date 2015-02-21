// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - A void method cannot return type int
 */
public class Je_6_Assignable_Return_Void {

    public Je_6_Assignable_Return_Void () {}

    public void m() {
	return 17;
    }

    public static int test() {
        return 123;
    }

}
