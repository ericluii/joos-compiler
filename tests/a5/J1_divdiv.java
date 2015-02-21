// CODE_GENERATION
public class J1_divdiv {
    public J1_divdiv() {}
    public static int test() {
	return 123 + J1_divdiv.div(2147483647) + 10*J1_divdiv.div2(1234);
    }

    public static int div(int x) {
	return x / 2147483647 / 2;
    }

    public static int div2(int x) {
	return x / -2147483648 / 2;
    }
}
