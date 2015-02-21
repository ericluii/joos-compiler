// CODE_GENERATION
public class J1_toomuchinc {
    public J1_toomuchinc() {}
    public static int test() {
	int x = -200000+123;
	new J1_toomuchinc();
	x = x+100000;
	x = 100000+x;
	new J1_toomuchinc();
	return x;
    }
}
