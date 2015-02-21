// CODE_GENERATION
public class J1_sideeffect1 {
    public J1_sideeffect1() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect1().test2();
    }
    public int test2() {
	g=1;
	int r1 = f(1) + f(10) * -(f(100) - f(1000));
	return r1 - 137879;
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
