// TYPE_CHECKING
public class J1_typecheck_instanceof3 {

    public J1_typecheck_instanceof3 () {}

    public static int test() {
	boolean b = true;
	b = (new Object[0] instanceof Object);
	
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
