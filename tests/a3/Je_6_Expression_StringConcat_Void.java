// JOOS1:TYPE_CHECKING,BINOP_TYPE
// JOOS2:TYPE_CHECKING,BINOP_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - + operator cannot be applied to String and void
 */
public class Je_6_Expression_StringConcat_Void {

    public Je_6_Expression_StringConcat_Void () {}

    public void m() {}

    public static int test() {
	String s = "flim"+new Je_6_Expression_StringConcat_Void().m();
        return 123;
    }

}
