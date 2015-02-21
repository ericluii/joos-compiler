// JOOS1:TYPE_CHECKING,BINOP_TYPE
// JOOS2:TYPE_CHECKING,BINOP_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type void not allowed in equality test
 */
public class Je_6_Equality_Void {

    public Je_6_Equality_Void () {}

    public void foo() {}

    public void bar() {}

    public static int test() {
	Je_6_Equality_Void j = new Je_6_Equality_Void();
	if (j.foo() == j.bar())
	    return 123;
	else
	    return 7;
    }

}
