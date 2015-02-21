package foo;

public class Bar {
	public Bar() {}
	
	public static int method() {
		return Main.Bar.method(); // Main is not visible => Main.Bar is a qualified type
	}
}
