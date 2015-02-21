// JOOS1:TYPE_CHECKING,BINOP_TYPE
// JOOS2:TYPE_CHECKING,BINOP_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type String cannot be equality compared to type Integer
 */
public class Je_6_Equality_StringInteger {

    public Je_6_Equality_StringInteger() {}

    public static int test() {
	String s = "flimflam";
	Integer i = new Integer(123);
	boolean b = (s==i);
        return 123;
    }

}
