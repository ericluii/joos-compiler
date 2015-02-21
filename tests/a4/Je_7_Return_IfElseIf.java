// JOOS1:REACHABILITY,MISSING_RETURN_STATEMENT
// JOOS2:REACHABILITY,MISSING_RETURN_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - If the body of a method whose return type is not void can
 * complete normally, produce an error message.  
 */
public class Je_7_Return_IfElseIf {
	
    public Je_7_Return_IfElseIf(){}
    
    public String test(int foo) {
	if (foo == 1) return "1";
	else if (foo == 2) return "2";
    }

}
