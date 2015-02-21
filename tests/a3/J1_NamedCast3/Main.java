// TYPE_CHECKING
/* TypeChecking:
 * class A {
 *	int testA()
 * }
 * class Main extends A {
 *	int testA()
 * }
 */

public class Main extends A {

    public Main(){}

    public static int test() {
	
	A a = new A();
	int result = 123;
	Main b = null;

	if (a.testA() > 300){
	    b = (Main) a; // cast is legal (but would result in a ClassCastException)
	    result = 723892;
	}
	return result;
	
    }

    public int testA(){
	return 120;
    }
}
