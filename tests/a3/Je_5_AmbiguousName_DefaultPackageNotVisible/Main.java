//JOOS1:DISAMBIGUATION,VARIABLE_OR_TYPE_NOT_FOUND
//JOOS2:DISAMBIGUATION,VARIABLE_OR_TYPE_NOT_FOUND
//JAVAC:UNKNOWN

public class Main {
	public Main() {}
	
	public static int test() {
		return foo.Bar.method();
	}
	
	public static int method() {
		return 123;
	}
}
