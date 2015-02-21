// DISAMBIGUATION
public class J1_methodInvocationQualified {

    public int bar() {
	return 123;
    }

    public J1_methodInvocationQualified() {}

    public static int test() {
	J1_methodInvocationQualified foo = new J1_methodInvocationQualified();
	return foo.bar();
    }

}
