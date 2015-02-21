// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (Too few arguments)
 */
public class Je_6_ConstructorPresent_TooFewArguments {

    public Je_6_ConstructorPresent_TooFewArguments(String s, String t) {}

    public static int test() {
        new Je_6_ConstructorPresent_TooFewArguments("If read this you can , I will help you not!");
	return 123;
    }

}
