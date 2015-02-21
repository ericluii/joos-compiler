// DISAMBIGUATION
public class J1_implicitthisforfields {
    
    public J1_implicitthisforfields() {}
    
    protected int x = 17;
    
    public int m() {
	x = 123;
	return x;
    }
    
    public static int test() {
	return new J1_implicitthisforfields().m();
    }



}

