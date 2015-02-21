// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (One mismatching argument)
 */
public class Je_6_MethodPresent_MultipleArgumentsOneMismatch {

    public Je_6_MethodPresent_MultipleArgumentsOneMismatch() {}

    public static int test() {
        return new Je_6_MethodPresent_MultipleArgumentsOneMismatch().m(42, new Object(), true, "If you can read this, it is your destiny.");
    }

    public int m(int a, String b, boolean t, String c ) {
        return 123;
    }

}
