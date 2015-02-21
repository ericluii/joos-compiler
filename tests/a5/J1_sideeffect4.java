// CODE_GENERATION
public class J1_sideeffect4 {
    public J1_sideeffect4() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect4().test2();
    }
    public int test2() {
	g=1;
	boolean b4 = (f(1)==f(1) | f(2)==f(2));
        int r4 = 0; if (b4) r4=f(10); else r4=f(100);
        return r4 - 477;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
