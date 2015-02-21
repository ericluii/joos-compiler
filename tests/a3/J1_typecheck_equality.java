// TYPE_CHECKING
public class J1_typecheck_equality {

    public J1_typecheck_equality () {}

    public static int test() {
	Object o = new Object();
	boolean b = true;
	b = (1 == 1)
	    &&
	    !(1 == 2)
	    &&
	    (true == true)
	    &&
	    !(true == false)
	    &&
	    (o == o)
	    &&
	    !(o == new Object());
	if (b)
	    return 123;
	else
	    return 7;
    }

}
