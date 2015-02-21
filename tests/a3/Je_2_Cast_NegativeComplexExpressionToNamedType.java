// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Environments:
 * - Variable Object not declared
 */
public class Je_2_Cast_NegativeComplexExpressionToNamedType {

    public Je_2_Cast_NegativeComplexExpressionToNamedType() {}

    public static int test() {
	int a = 5;
	int x = (Object)-a+117;
	return 123;
    }

}
