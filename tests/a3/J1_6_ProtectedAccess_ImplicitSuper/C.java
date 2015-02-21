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
    /* implicit call to protected constructor of B
     * => OK, since C is a subclass of B (6.6.2.2 item 1)
     */
    public C() {} 
}
