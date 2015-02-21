// TYPE_CHECKING
public class J1_typecheck_constructor_invocation {

    public J1_typecheck_constructor_invocation (int i, String s) { 
    }

    public static int test() {
	new J1_typecheck_constructor_invocation(7, "flimflam");
        return 123;
    }

}
