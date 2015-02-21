// TYPE_CHECKING
/*
 * TypeChecking:
 * Main extends A and thus Main[] is assignable to A[]
 */
public class Main extends A {

    public Main(){}

    public static int test() {
	
	Main[] a = new Main[42];
	a[0] = new Main();

	int result = 12456;
	A[] b = null;

	b = (A[]) a;
	result = b[0].testA();

	return result;
	
    }

    public int testA(){
	return 123;
    }
}
