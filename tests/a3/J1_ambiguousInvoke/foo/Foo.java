package foo;

import foo.*;

public class Foo {

    public static void bar() {
	System.out.println("bar");
    }

    public Foo() {}

    public void test() {
	foo.Foo.bar();
    }

    public static void main(String[] args) {
	new foo.Foo().test();
    }
}
