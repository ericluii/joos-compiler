// DISAMBIGUATION
import foo.*;

public class Main {
    
    public Main() {}

    public static int test() {
	Foo foo = new Foo();
	foo.test();
	return 123;
    }
}
