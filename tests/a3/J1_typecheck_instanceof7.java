// TYPE_CHECKING
public class J1_typecheck_instanceof7 {

    public J1_typecheck_instanceof7 () {}

    public static int test() {
	boolean b = true;
	b = !(new Object() instanceof Number);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
