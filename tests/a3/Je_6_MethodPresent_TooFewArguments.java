// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (Too few arguments)
 */
public class Je_6_MethodPresent_TooFewArguments {

    public Je_6_MethodPresent_TooFewArguments() {}

    public static int test() {
        return new Je_6_MethodPresent_TooFewArguments().m("If you can read this, you are one step closer to passing the dOvs course.");
    }

    public int m(String s, String t) {
        return 123;
    }

}
