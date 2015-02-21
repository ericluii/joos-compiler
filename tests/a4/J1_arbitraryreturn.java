// REACHABILITY
public class J1_arbitraryreturn {
    public J1_arbitraryreturn() {}
    public int m(int x) {
	if (x==42) 
	    return 123;
	else
	    return 117;
    }
    public static int test() {
	J1_arbitraryreturn obj = new J1_arbitraryreturn();
	int y = obj.m(42);
	if (y!=123) {
	    return 42;
	}
	else {
	    return y;
	}
    }
}

