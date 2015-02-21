// CODE_GENERATION
public class J1_sideeffect7 {
    public J1_sideeffect7() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect7().test2();
    }
    public int test2() {
	g=1;
	boolean b7 = (f(1)==2 || 10/0<10);
        int r7 = String.valueOf(b7).charAt(0);
        return r7 + 7;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
