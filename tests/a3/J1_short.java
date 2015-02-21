// TYPE_CHECKING
public class J1_short {
    public J1_short() {}
    protected short x = (short)123;
    public static int test() {
	J1_short obj = new J1_short();
	short y = obj.x;
	return (int)y;
    }
}

