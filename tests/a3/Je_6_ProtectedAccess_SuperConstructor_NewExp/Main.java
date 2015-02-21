//JOOS1:TYPE_CHECKING,PROTECTED_CONSTRUCTOR_INVOCATION,NO_MATCHING_CONSTRUCTOR_FOUND
//JOOS2:TYPE_CHECKING,PROTECTED_CONSTRUCTOR_INVOCATION,NO_MATCHING_CONSTRUCTOR_FOUND
//JAVAC:UNKNOWN

import foo.Bar;

public class Main extends Bar {
	public Main() {}
	
	public static int test() {
		Bar c = new Bar(); // protected constructor
		return 123;
	}
}
