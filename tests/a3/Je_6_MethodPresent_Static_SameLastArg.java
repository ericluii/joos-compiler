// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN

public class Je_6_MethodPresent_Static_SameLastArg {
	
	public Je_6_MethodPresent_Static_SameLastArg() {}
	
	public static int method(int a, String b) {
		return 123;
	}
	
	public static int test() {
		return Je_6_MethodPresent_Static_SameLastArg.method("a","b");
	}
}
