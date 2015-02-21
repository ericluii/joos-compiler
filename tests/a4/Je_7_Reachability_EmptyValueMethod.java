// JOOS1:REACHABILITY,MISSING_RETURN_STATEMENT
// JOOS2:REACHABILITY,MISSING_RETURN_STATEMENT
// JAVAC:UNKNOWN

public class Je_7_Reachability_EmptyValueMethod {
	public Je_7_Reachability_EmptyValueMethod() {}
	
	public int empty() {}
	
	public static int test() {
		return 123;
	}
}
