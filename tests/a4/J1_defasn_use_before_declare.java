// DEFINITE_ASSIGNMENT
public class J1_defasn_use_before_declare {
    public J1_defasn_use_before_declare() {}
    public static int test() {return new J1_defasn_use_before_declare().foo();}

    protected int a = 23;
    public int foo() {
	int tmp = a;
	int a = 100;
	return a+tmp;
    }
}
