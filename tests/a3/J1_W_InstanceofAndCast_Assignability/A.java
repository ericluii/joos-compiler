public class A {
    public A() {
        boolean b = new A() instanceof B;
        B c = (B) new A();
    }
}
