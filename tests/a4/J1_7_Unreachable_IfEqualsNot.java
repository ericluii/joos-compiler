// REACHABILITY
/**
 * Reachability:
 * - Check that all statements (including empty statements and empty
 * blocks) are reachable.  
 */
public class J1_7_Unreachable_IfEqualsNot {
	
    public J1_7_Unreachable_IfEqualsNot(){}

    public static int test(){
	return J1_7_Unreachable_IfEqualsNot.bar(true);
    }
    
    public static int bar(boolean foo) {
	if (foo == !foo) {
	    int h = 100;
	    return h;
	}
	return 123;
    }

}
