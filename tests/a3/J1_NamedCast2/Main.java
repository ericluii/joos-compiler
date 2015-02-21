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
	
	Main a = new Main();
	
	return ((A)a).testA();
	
    }

    public int testMain(){
	return 123;
    }
}
