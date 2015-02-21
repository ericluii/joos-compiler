// TYPE_CHECKING,CODE_GENERATION
public class J1_array {
    public J1_array() {}
    protected int[] x = new int[42];
    public static int test() {
	J1_array obj = new J1_array();
	obj.x[17] = 123;
	int[] y = obj.x;
	return y[17];
    }
}

