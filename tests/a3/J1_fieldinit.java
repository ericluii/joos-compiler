// DISAMBIGUATION,CODE_GENERATION
public class J1_fieldinit {

    public int foo = 42;

    public J1_fieldinit() {}

    public int bar() {
	if (foo == 42) return 123;
	return 7;
    }

    public static int test() {
	return new J1_fieldinit().bar();
    }

}
