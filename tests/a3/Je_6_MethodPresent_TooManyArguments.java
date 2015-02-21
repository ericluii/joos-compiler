// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (Too many arguments)
 */
public class Je_6_MethodPresent_TooManyArguments {

    public Je_6_MethodPresent_TooManyArguments() {}

    public static int test() {
        return new Je_6_MethodPresent_TooManyArguments().m("If you can read this, you are capable of reading.");
    }

    public int m() {
        return 123;
    }

}
