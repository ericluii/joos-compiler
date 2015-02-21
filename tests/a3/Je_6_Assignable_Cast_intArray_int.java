// JOOS1:PARSER_WEEDER,PARSER_EXCEPTION,TYPE_CHECKING,INVALID_CAST
// JOOS2:PARSER_WEEDER,PARSER_EXCEPTION,TYPE_CHECKING,INVALID_CAST
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type int cannot be cast to type int[]
 */
public class Je_6_Assignable_Cast_intArray_int {

    public Je_6_Assignable_Cast_intArray_int() {}

    public static int test() {
	int[] x = (int[])-42;
	return 123;
    }

}
