// TYPE_CHECKING
public class J1_implicitthisformethods {

    public J1_implicitthisformethods() {}
    
    public int m1() {
	return 123;
    }
    
    public int m2() {
	return m1();
    }
    
    public static int test() {
	return new J1_implicitthisformethods().m1();
    }  

}

