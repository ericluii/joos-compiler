//DISAMBIGUATION

public class Main {
	public Main() {}
	
	public static int test() {
		return foo.Bar.method();
	}
	
	public int method() {
		return 123;
	}

	public Bar Bar = new Bar();
}
