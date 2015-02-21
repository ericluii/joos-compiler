// CODE_GENERATION
public class J1_sideeffect5 {
    public J1_sideeffect5() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect5().test2();
    }
    public int test2() {
	g=1;
	boolean b5=(f(1)==f(1) & f(2)==f(2));
        int r5 = 0; if (b5) r5=f(10); else r5=f(100);
        return 723 - r5;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
