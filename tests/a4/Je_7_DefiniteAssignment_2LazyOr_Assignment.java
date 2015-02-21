// JOOS1:DEFINITE_ASSIGNMENT,JOOS1_OMITTED_LOCAL_INITIALIZER
// JOOS2:DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
// JAVAC:UNKNOWN

public class Je_7_DefiniteAssignment_2LazyOr_Assignment {
	public Je_7_DefiniteAssignment_2LazyOr_Assignment() {}
	
	public boolean method() {
		boolean j;
		boolean i;
		if ((j=false) || (i=true));
		return i; 
	}
	
	public static int test() {
		return 123;
	}
}
