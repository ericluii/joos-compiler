// TYPE_CHECKING
public class J1_typecheck_instanceof {

    public J1_typecheck_instanceof () {}

    public static int test() {
	boolean b = true;
	b = (new Object() instanceof Object)
	    &&
	    (new Object[0] instanceof Object[])
	    &&
	    (new Object[0] instanceof Object)
	    &&
	    !(null instanceof Object)
	    &&
	    !(new Object() instanceof Object[])
	    &&
	    (new Integer(17) instanceof Number)
	    &&
	    !(new Object() instanceof Number);
	if (b)    
	    return 123;
	else
	    return 17;
    }

}
