// JOOS1:HIERARCHY,TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:HIERARCHY,TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type Thread is not assignable to type Je_6_Assignable_ToSubtype
 */
public class Main extends Thread {

    public Main(){}

    public static int test() {
	Main a = null;
	a = new Thread();
	return 123;
    }

}
