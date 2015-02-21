// JOOS1:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/* TypeChecking:
 * 
 * Test for Protected Access
 * 
 * A.A declares protected_field as protected
 * Main extends A.A
 * B.B extends A.A
 * 
 * Main cannot access protected_field on B.B since Main is not a supertype of B.B.
 * (See JLS 6.6.2.1) 
 */
public class Main extends A.A {
	public Main() {}
	
	public static int test() {
		B.B b= new B.B();
		return b.protected_field;
	}
}
