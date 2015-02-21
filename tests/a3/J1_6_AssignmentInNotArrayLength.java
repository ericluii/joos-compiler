// TYPE_CHECKING
public class J1_6_AssignmentInNotArrayLength {
	public J1_6_AssignmentInNotArrayLength() {}
	
	public int length = 123;
	
	public static int test() {
		J1_6_AssignmentInNotArrayLength a = null;
		return (a = new J1_6_AssignmentInNotArrayLength()).length;
	}
}
