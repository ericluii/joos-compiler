// TYPE_CHECKING
public class J1_typecheck_instanceof5 {

    public J1_typecheck_instanceof5 () {}

    public static int test() {
	boolean b = true;
	b = !(new Object() instanceof Object[]);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
