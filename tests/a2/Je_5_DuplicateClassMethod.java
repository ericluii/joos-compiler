public class Je_5_DuplicateClassMethod {

    protected int field;

    public Je_5_DuplicateClassMethod() {
    }

    public Je_5_DuplicateClassMethod(int a, int b, int c, int d, int e) {
	this.field = foo(a,b,c,d,e);
    }

    protected int foo(int a, int b, int c, int d, int e) {
	int local = a * c;
	return local+e;
    }

    protected int foo(int a, int b, int c, int d, int e) {
	int local = a * c;
	return local+e;
    }

    public static int test() {
        return new Je_5_DuplicateClassMethod(11,42,2,88,101).field;
    }

}
