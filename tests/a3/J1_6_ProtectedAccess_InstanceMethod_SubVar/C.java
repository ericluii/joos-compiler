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

    /* instance method access through subclass variable
     * => OK, since D.D (the type of var) is a subclass of C (6.6.2.1 item 1)
     */
    public void instanceMethodAccessFromSub(D.D var) {
	var.instanceMethod();
    }
}
