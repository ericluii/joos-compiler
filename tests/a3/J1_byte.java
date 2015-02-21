// TYPE_CHECKING
public class J1_byte {
    public J1_byte() {}
    protected byte x = (byte)123;
    public static int test() {
	J1_byte obj = new J1_byte();
	byte y = obj.x;
	return (int)y;
    }
}

