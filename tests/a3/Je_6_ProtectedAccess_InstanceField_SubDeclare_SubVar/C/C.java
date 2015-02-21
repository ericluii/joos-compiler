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

    /* instance field access through subclass variable
     * => FAIL, since D.D (the type of declaring .instanceField_Sub) is a not a superclass of C (6.6.2.1 item 2)
     */
    public void instanceFieldAccessFromSuper(D.D var, int x) {
	var.instanceField_Sub = x; 
    }
}
