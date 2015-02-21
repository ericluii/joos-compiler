// TYPE_CHECKING
public class J1_typecheck_instanceof1 {

    public J1_typecheck_instanceof1 () {}

    public static int test() {
	boolean b = true;
	b = (new Object() instanceof Object);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
