// JOOS1:REACHABILITY,MISSING_RETURN_STATEMENT
// JOOS2:REACHABILITY,MISSING_RETURN_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - If the body of a method whose return type is not void can
 * complete normally, produce an error message.  
 */
public class Je_7_Return_MissingInElse {
	
    public Je_7_Return_MissingInElse(){}
    
    public String test(int foo) {
	if (foo == 1) return "1";
	else if (foo == 2) return "2";
	else {
	    foo = 3;
	}
    }

}
