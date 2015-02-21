// HIERARCHY,TYPE_CHECKING
/* Hierarchy:
 * class CompA {
 *	int compareTo(Object) 
 * }
 * abstract class CompB extends CompA implements Comparable {
 *	abstract int compareTo(Object)
 * }
 * class CompC extends CompB {
 *  int compareTo(Object)
 * }
 */
public class Main {
    public Main() {}
    public static int test() {
	CompB x = new CompC();
	return x.compareTo((Object)null);
    }
}
