// JOOS1:REACHABILITY,MISSING_RETURN_STATEMENT
// JOOS2:REACHABILITY,MISSING_RETURN_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - If the body of a method whose return type is not void can
 * complete normally, produce an error message.  
 */
public class Je_7_Return_IfIfNot {
    
    public Je_7_Return_IfIfNot(){}
    
    public int test(boolean foo) {
	if (foo) return 17;
	if (!foo) return 42;
    }

}
