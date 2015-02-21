// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type Thread is not assignable to type
 * Je_6_Assignable_ToSubtype_DeclInit
 */
public class Main extends Thread {

    public Main(){}
    
    public static int test() {
	Main a = new Thread();
	return 123;
    }

}
