// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN

public class Je_5_ForwardReference_InAssignment {
	public Je_5_ForwardReference_InAssignment() {}
	
	public int a = Je_5_ForwardReference_InAssignment.method(a).a = 0;
	
	public static Je_5_ForwardReference_InAssignment method(int a) {
		return null;
	}
	
	public static int test() {
		return 123;
	}
}
