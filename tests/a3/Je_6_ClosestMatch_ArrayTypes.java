// TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
/**
 * Typecheck:
 * - (Joos 1) The declared types of the method or constructor have to
 * match exactly the static types of the argument expressions.
 * - (Joos 2) Closest-match overloading as described in Section
 * 15.12.2 of the JLS. (Closest matching not applicable for array types)
 */
public class Je_6_ClosestMatch_ArrayTypes {

    public Je_6_ClosestMatch_ArrayTypes (int[] i) {}

    public static int test() {
	char[] c = new char[2];
	Je_6_ClosestMatch_ArrayTypes j = new Je_6_ClosestMatch_ArrayTypes(c);
        return 123;
    }

}
