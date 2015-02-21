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

    /* instance field access through this class variable
     * => OK, since C (the type of var) is a subclass of C (6.6.2.1 item 1)
     */
    public void instanceFieldAccessFromVar(C var, int x) {
	var.instanceField = x;
    }
}
