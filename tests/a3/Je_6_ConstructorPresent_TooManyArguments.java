// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (Too many arguments)
 */
public class Je_6_ConstructorPresent_TooManyArguments {

    public Je_6_ConstructorPresent_TooManyArguments() {}

    public static int test() {
        new Je_6_ConstructorPresent_TooManyArguments("If only Yoda could see you now.");
	return 123;
    }

}
