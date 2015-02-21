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

    /* instance method access through superclass variable
     * => FAIL, since D.D (the type declaring instanceMethod_Sub) is not a superclass of C (6.6.2.1 item 2)
     */
    public void instanceMethodAccessFromThis(D.D var) {
	var.instanceMethod_Sub();
    }
}
