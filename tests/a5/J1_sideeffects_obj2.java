// CODE_GENERATION
public class J1_sideeffects_obj2 {
    protected int foo = 0;

    public J1_sideeffects_obj2(int foo) {this.foo=foo;}

    public static int test() {
	J1_sideeffects_obj2 o1 = new J1_sideeffects_obj2(100);
	J1_sideeffects_obj2 o2 = new J1_sideeffects_obj2(-10);
	J1_sideeffects_obj2 o3 = new J1_sideeffects_obj2(3);

	J1_sideeffects_obj2 tmp = o2;
	// o1:100 o2:-10 o3:3 tmp=o2
	tmp.foo = tmp.foo + 10*(tmp = o3).foo;
	// o1:100 o2:20 o3:3 tmp=o3
	
	return o1.foo + o2.foo + tmp.foo;
    }

}
