// JOOS1:PARSER_WEEDER,PARSER_EXCEPTION,JOOS1_STATIC_FIELD_DECLARATION
// JOOS2:CODE_GENERATION
public class J2_A_FieldInitialization_Static_NonConstant_Before {
	public static int b = J2_A_FieldInitialization_Static_NonConstant_Before.a; // b = 0
	public static int a = new Integer(22).intValue(); // a = 22

	public J2_A_FieldInitialization_Static_NonConstant_Before() {}
	
	public static int test() {
		return J2_A_FieldInitialization_Static_NonConstant_Before.a + J2_A_FieldInitialization_Static_NonConstant_Before.b + 101;
	}
}
