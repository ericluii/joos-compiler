// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (No matching method for 
 * invocation of m(Je_6_MethodPresent_ArgumentTypeMismatch))
 */
public class Je_6_MethodPresent_ArgumentTypeMismatch {

    public Je_6_MethodPresent_ArgumentTypeMismatch() {}

    public static int test() {
        return new Je_6_MethodPresent_ArgumentTypeMismatch().m(new Je_6_MethodPresent_ArgumentTypeMismatch());
    }

    public int m(String s) {
        return 123;
    }

}
