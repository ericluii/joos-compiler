// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Check that all fields, methods and constructors that are to be
 * linked as described in the decoration rules are actually present in
 * the corresponding class or interface. (One mismatching argument)
 */
public class Je_6_ConstructorPresent_MultipleArgumentsOneMismatch {

    public Je_6_ConstructorPresent_MultipleArgumentsOneMismatch(int a, String b, boolean t, String c) {}

    public static int test() {
        new Je_6_ConstructorPresent_MultipleArgumentsOneMismatch(42, new Object(), true, "If you can read this, world peace is not here yet.");
	return 123;
    }

}
