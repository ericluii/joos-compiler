// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (Method testMain not present
 * in class A).
 */
public class Main extends A {

    public Main(){}

    public static int test() {
	Main a = new Main();
	return ((A)a).testMain();
    }

    public int testMain(){
	return 120;
    }
}
