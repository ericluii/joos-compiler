// JOOS1:DEFINITE_ASSIGNMENT,JOOS1_OMITTED_LOCAL_INITIALIZER
// JOOS2:DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
// JAVAC:UNKNOWN

public class Je_7_DefiniteAssignment_3LazyOr_Assignment {
	public Je_7_DefiniteAssignment_3LazyOr_Assignment() {}
	
	public boolean method() {
		boolean a;
		boolean b;
		boolean c;
		boolean d=(a=false) || ((b=false) || (c=false));
		return c; 
	}
	
	public static int test() {
		return 123;
	}
}
