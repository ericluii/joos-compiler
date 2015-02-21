// DISAMBIGUATION
public class J1_samestaticinvoketwice {

    protected J1_samestaticinvoketwice() {}

    public static int test() {
	System.gc();
	System.gc();
	return 123;
    }

}
