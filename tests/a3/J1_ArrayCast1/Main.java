// TYPE_CHECKING
/*
 * TypeChecking:
 * Main extends A and thus A[] can be cast to Main[]
 */
public class Main extends A {

    public Main(){}

    public static int test() {
	
	A[] a = new A[42];
	a[0] = new A();

	int result = 123;
	Main[] b = null;

	if (a[0].testA() > 300) {
	    b = (Main[]) a;
	    result = 236728;
	}

	return result;
	
    }

    public int testA(){
	return -123;
    }
}
