// JOOS1:PARSER_WEEDER,DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:PARSER_WEEDER,DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
/**
 * Disambiguation:
 * - Looks like a cast expression, but must be parsed as a
 *   parenthesized expression. 
 *   Je_1_Cast_NamedCastNegativeint is not a field or local variable.
 */
public class Je_1_Cast_NamedCastNegativeint {

    public Je_1_Cast_NamedCastNegativeint () {}

    public static int test () {
	return (Je_1_Cast_NamedCastNegativeint)-123;
    }
}
