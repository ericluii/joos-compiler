// DISAMBIGUATION
public class J1_int {
    public J1_int() {}
    protected int x = 123;
    public static int test() {
	J1_int obj = new J1_int();
	int y = obj.x;
	return y;
    }
}
