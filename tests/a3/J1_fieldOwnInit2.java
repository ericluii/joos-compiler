// DISAMBIGUATION
public class J1_fieldOwnInit2 {
	public int f = 1 + 3 + (f=(f=2));
	public J1_fieldOwnInit2() { }
	public static int test() { return 123; }
}
