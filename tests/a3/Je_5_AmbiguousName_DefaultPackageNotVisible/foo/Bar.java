package foo;

public class Bar {
	public Bar() {}
	
	public static int method() {
		return Main.method(); // Main is not visible
	}
}
