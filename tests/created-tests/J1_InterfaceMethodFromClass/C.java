/* The class B has method someMethod() from
 * interface A which it implements. The linked method
 * for b.someMethod() is the interface method, someMethod()
 * of C */

public class C extends B {
    public int someMethod() {
        return 2;
    }

    public C() {
        B b = new C();
        b.someMethod();
    }
}
