/* TypeChecking:
 * 
 * Test for Protected Access
 * 
 * B.B extends A.A
 * C.C extends B.B
 * D.D extends C.C
 */

package C;

public class C extends B.B {
    public C() {}

    /* static field access through superclass
     * => OK, since the current type C is a subclass of the declaring type A.A (6.6.2.1)
     */ 
    public void staticFieldAccessFromSuperClass(int x) {
	A.A.staticField = x;
    }
}
