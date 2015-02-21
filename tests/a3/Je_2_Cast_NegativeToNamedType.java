// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Environment:
 * - Variable Object not declared
 */
public class Je_2_Cast_NegativeToNamedType {

    public Je_2_Cast_NegativeToNamedType() {}

    public static int test() {
	int x = (Object)-42;
	return 123;
    }

}
