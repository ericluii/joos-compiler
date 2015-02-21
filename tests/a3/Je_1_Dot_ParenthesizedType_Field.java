// JOOS1:PARSER_WEEDER,DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:PARSER_WEEDER,DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * Dots not allowed on parenthesized types, the
 * expression must parsed as an ambiguous name and
 * Integer does not resolve to a field or local variable.
 */
public class Je_1_Dot_ParenthesizedType_Field {

    public Je_1_Dot_ParenthesizedType_Field(){}

    public static int test() {
	int foo = (Integer).MAX_VALUE;
	return foo/17459216;
    }

}
