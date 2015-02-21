// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are
 * to be linked as described in the decoration rules are actually
 * present in the corresponding class or interface. (Constructor
 * Main() implicitly contains a call to super())
 */
public class Main extends Foo {

    public Main() {
    }
    
    public static int test() {
	return 123;
    }
    
}
