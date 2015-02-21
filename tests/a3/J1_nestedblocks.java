// DISAMBIGUATION
public class J1_nestedblocks {
    public J1_nestedblocks() {}
    public int m() {
	int x = 22;
	int y = 101;
	{
	    int z = x+y;
	    return z;
	}
    }
    public static int test() {
	return new J1_nestedblocks().m();
    }
}
