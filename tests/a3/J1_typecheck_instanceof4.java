// TYPE_CHECKING
public class J1_typecheck_instanceof4 {

    public J1_typecheck_instanceof4 () {}

    public static int test() {
	boolean b = true;
	b = !(null instanceof Object);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
