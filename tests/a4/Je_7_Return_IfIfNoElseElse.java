// JOOS1:REACHABILITY,MISSING_RETURN_STATEMENT
// JOOS2:REACHABILITY,MISSING_RETURN_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - If the body of a method whose return type is not void can
 * complete normally, produce an error message
 */
public class Je_7_Return_IfIfNoElseElse {

    public Je_7_Return_IfIfNoElseElse () {}

    public int m() {
	boolean t = true;

	if (t) { 
	    if (t) {
		return 117;
	    }
	}
	else {
	    return 42;
	}
    }

    public static int test() {
        return 123;
    }

}
