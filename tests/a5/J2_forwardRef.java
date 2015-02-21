// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION,CODE_GENERATION
public class J2_forwardRef {
	public int a = J2_forwardRef.b;
	public static int b = 2;
	
	public J2_forwardRef() {
	}

	public static int test() {
		return 121 + J2_forwardRef.b;
	}
}
