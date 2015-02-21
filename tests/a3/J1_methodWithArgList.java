// TYPE_CHECKING
public class J1_methodWithArgList {

    public J1_methodWithArgList () {}

    public int foo(int i, String s, J1_methodWithArgList j) {
	if (i == 123) {
	    return i;
	}
	return 7;
    }

    public static int test() {
	J1_methodWithArgList j = new J1_methodWithArgList();
        return j.foo(123, "foo", j);
    }

}
