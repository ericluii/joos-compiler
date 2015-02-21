// JOOS1:TYPE_CHECKING,BINOP_TYPE
// JOOS2:TYPE_CHECKING,BINOP_TYPE
// JAVAC:UNKNOWN
// 
public class Je_6_StringMinus {
    	/* TypeChecking => minus on strings not allowed */
	public Je_6_StringMinus() {}
	
	public static int test() {
		String s = "nu"-"ll";
		return 127-s.length();
	}
}
