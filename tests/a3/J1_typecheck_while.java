// TYPE_CHECKING
public class J1_typecheck_while {

    public J1_typecheck_while () {}

    public static int test() {
	int i = 5;
	while (i!=5) 
	    i = i+1; 
	while (i>0 && i < 17) { 
	    i = i + 1;
	}
	while (true)
	    return 123;
    }

}
