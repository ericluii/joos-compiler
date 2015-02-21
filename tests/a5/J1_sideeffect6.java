// CODE_GENERATION
public class J1_sideeffect6 {
    public J1_sideeffect6() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect6().test2();
    }
    public int test2() {
	g=1;
        int r6 = f(1)*0 + f(10) * 1 + (f(100)-f(100));
        return -17-2*r6;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
