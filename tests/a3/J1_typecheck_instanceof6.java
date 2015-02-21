// TYPE_CHECKING
public class J1_typecheck_instanceof6 {

    public J1_typecheck_instanceof6 () {}

    public static int test() {
	boolean b = true;
	b = (new Integer(17) instanceof Number);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
