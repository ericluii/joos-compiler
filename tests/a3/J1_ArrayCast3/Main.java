// TYPE_CHECKING
/*
 * TypeChecking:
 * Main extends A and thus Main[] is assignable to A[]
 */
public class Main extends A {

    public Main(){}

    public static int test() {
	
	A[] a = new Main[42];
	
	int result = 123;

	if (result > 142 ){
	    a[0] = new A();
	}

	return result;
	
    }

    public int testA(){
	return 120;
    }
}
