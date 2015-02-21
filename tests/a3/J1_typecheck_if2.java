// TYPE_CHECKING
public class J1_typecheck_if2 {

    public J1_typecheck_if2 () {}

    public static int test() {
	Object obj = new Object();
	if (obj==obj) return 123;
        return 7;
    }

}
