// JOOS1:REACHABILITY,UNREACHABLE_STATEMENT
// JOOS2:REACHABILITY,UNREACHABLE_STATEMENT
// JAVAC:UNKNOWN
// 
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.
 */
public class Je_7_Reachability_WhileFalse_Empty {

    public Je_7_Reachability_WhileFalse_Empty() { }

    public static int test() {
	while(false);
	return 123;
    }
}
