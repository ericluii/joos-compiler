// TYPE_CHECKING
public class J1_methodoverloading {

    public J1_methodoverloading() {}

    public int m1(int x) {
	return x;
    }
    
    public int m1(boolean x) {
	return 87;
    }
    
    public int m2() {
	return this.m1(true)+this.m1(36);
    }
    
    public static int test() {
	return new J1_methodoverloading().m2();
    }

}
