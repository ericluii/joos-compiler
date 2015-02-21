public class J1_evalMethodInvocation {

    public int foo() {
	return this.bar();
    }

    public int bar() {
	return 123;
    }

    public J1_evalMethodInvocation() {}


}
