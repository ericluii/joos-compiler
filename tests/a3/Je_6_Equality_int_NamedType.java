// JOOS1:TYPE_CHECKING,BINOP_TYPE
// JOOS2:TYPE_CHECKING,BINOP_TYPE
// JAVAC:
/**
 * Typecheck:
 * - Type Object cannot be equality compared to type int
 */
public class Je_6_Equality_int_NamedType {

    public Je_6_Equality_int_NamedType() {}

    public static int test() {
	Object o = new Object();
	boolean b = (1==o);
        return 123;
    }

}
