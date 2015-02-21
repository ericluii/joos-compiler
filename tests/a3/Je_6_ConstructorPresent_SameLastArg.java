// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN

public class Je_6_ConstructorPresent_SameLastArg {
	
	public Je_6_ConstructorPresent_SameLastArg(int a, String b) {}
	
	public int method() {
		return 123;
	}
	
	public static int test() {
		Je_6_ConstructorPresent_SameLastArg j = new Je_6_ConstructorPresent_SameLastArg("a","b");
		return j.method();
	}
}
