// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND,PROTECTED_CONSTRUCTOR_INVOCATION
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND,PROTECTED_CONSTRUCTOR_INVOCATION
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
