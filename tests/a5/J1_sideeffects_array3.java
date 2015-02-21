// CODE_GENERATION
public class J1_sideeffects_array3 {
    public J1_sideeffects_array3() {}
    public static int test() {
	J1_sideeffects_array3 obj = new J1_sideeffects_array3();
	obj.foo()[obj.bar()] = obj.baz();
	return obj.cpy[2] + 2*obj.b;
    }


    protected int[] cpy = null;
    protected int[] a = new int[3];
    protected int b = 0;

    public int[] foo() { b = 2; return a; }
    public int bar() { cpy = a; a = null; return b;}
    public int baz() { a = null; b = 50; return 23;}
}
