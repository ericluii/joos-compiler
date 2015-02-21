// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND,PROTECTED_MEMBER_ACCESS
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND,PROTECTED_MEMBER_ACCESS
// JAVAC:UNKNOWN
// 
/* TypeChecking:
 * 
 * Test for Protected Access
 * 
 * B.B extends A.A
 * C.C extends B.B
 * D.D extends C.C
 * 
 * see C.java
 */
public class Main {
    public Main() {}
    
    public static int test() {
	return 123;
    }
}
