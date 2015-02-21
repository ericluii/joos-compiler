// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - A value return statement cannot have type void,
 *   not even in a void method.
 */
public class Je_6_Assignable_Return_VoidInVoidMethod {

    public Je_6_Assignable_Return_VoidInVoidMethod () {}

    public void m() {
	return foo();
    }

    public void foo(){}

    public static int test() {
        return 123;
    }

}
