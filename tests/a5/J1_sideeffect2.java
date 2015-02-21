// CODE_GENERATION
public class J1_sideeffect2 {
    public J1_sideeffect2() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect2().test2();
    }
    public int test2() {
	g=1;
	boolean b2 = (f(1)==f(1) || f(2)==f(2));
        int r2 = 0; if (b2) r2=f(10); else r2=f(100);
	return r2 - 477;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
