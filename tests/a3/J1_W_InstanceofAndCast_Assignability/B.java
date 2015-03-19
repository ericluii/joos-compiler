public class B extends A {
    public B() {
        boolean b = new B() instanceof A;
        A c = (A) new B();
    }
}
