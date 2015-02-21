// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Environment:
 * - Variable java.lang.Object not declared
 */
public class Je_2_Cast_NegativeToQualifiedNamedType {

    public Je_2_Cast_NegativeToQualifiedNamedType() {}

    public static int test() {
	int x = (java.lang.Object)-42;
	return 123;
    }

}
