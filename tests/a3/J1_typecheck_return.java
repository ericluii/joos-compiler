// TYPE_CHECKING
public class J1_typecheck_return {

    public J1_typecheck_return () {}

    public void m() {
	return;
    }

    public Object n() {
	if (true) return "foobar="+17;
	else return new Object(); 
    }

    public static int test() {
	J1_typecheck_return tc = new J1_typecheck_return();
	tc.m();
	tc.n();
        return 123;
    }

}
