// JOOS1:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,PROTECTED_MEMBER_ACCESS,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN
// 
/* TypeChecking:
 * 
 * Test for Protected Access
 * 
 * p.A declares protected_field as protected
 * and the class is not a subtype of p.A nor is it a supertype of p.B
 * (See JLS 6.6.2) 
 */
import p.*;

public class Main {
    public Main() {}
    
    public static int test() {
	B b = new B();
	return b.protected_field;
    }
}
