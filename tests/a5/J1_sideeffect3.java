// CODE_GENERATION
public class J1_sideeffect3 {
    public J1_sideeffect3() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect3().test2();
    }
    public int test2() {
	g=1;
	boolean b3 = (f(1)==f(1) && f(2)==f(2));
        int r3 = 0; if (b3) r3=f(10); else r3=f(100);
        return r3 - 277;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
