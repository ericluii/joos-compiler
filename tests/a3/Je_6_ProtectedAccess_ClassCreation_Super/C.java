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

    /* constructor access through this
     * => FAIL, since A is not in the package of C (6.6.2.2 item 3)
     */
    public void constructorAccessCreationOfSuperType() {
	new A.A();
    }
}
