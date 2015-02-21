// TYPE_CHECKING
// JOOS1: JOOS1_IMPLICIT_THIS_CLASS_STATIC_METHOD

import pac.Foo;

public class Main extends Foo {
	public Main() {
	}
	
	public static int test() {
		return Main.bar();
	}
}
