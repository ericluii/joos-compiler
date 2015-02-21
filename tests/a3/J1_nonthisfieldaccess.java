// DISAMBIGUATION
public class J1_nonthisfieldaccess {
    
    public J1_nonthisfieldaccess() {}
    
    public int x;
    
    public static int test() {
	J1_nonthisfieldaccess obj = new J1_nonthisfieldaccess();
	obj.x = 123;
	return obj.x;
    }
    
}
