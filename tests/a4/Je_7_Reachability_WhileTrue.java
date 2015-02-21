// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.
 */
public class Je_7_Reachability_WhileTrue {

    public Je_7_Reachability_WhileTrue() {}

    public static int test() {
	while (true) { }
	return 123;
    }

}
