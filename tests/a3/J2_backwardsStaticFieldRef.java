// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION
public class J2_backwardsStaticFieldRef {
	public static int field2 = 100;
	public static int field1 = 23 + J2_backwardsStaticFieldRef.field2;
	public J2_backwardsStaticFieldRef() { }
	public static int test() { return J2_backwardsStaticFieldRef.field1; }
}
