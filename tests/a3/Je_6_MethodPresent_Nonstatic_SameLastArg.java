// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JAVAC:UNKNOWN

public class Je_6_MethodPresent_Nonstatic_SameLastArg {
	
	public Je_6_MethodPresent_Nonstatic_SameLastArg() {}
	
	public int method(int a, String b) {
		return 123;
	}
	
	public static int test() {
		Je_6_MethodPresent_Nonstatic_SameLastArg j = new Je_6_MethodPresent_Nonstatic_SameLastArg();
		return j.method("a","b");
	}
}
