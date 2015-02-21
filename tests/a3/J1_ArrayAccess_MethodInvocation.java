// TYPE_CHECKING
public class J1_ArrayAccess_MethodInvocation {
	public J1_ArrayAccess_MethodInvocation() {
		
	}
	
	public static int test() {
		Integer[] i = new Integer[1];
		i[0] = new Integer(123);
		return i[0].intValue();
	}
}
