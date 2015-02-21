//JOOS1:DEFINITE_ASSIGNMENT,JOOS1_OMITTED_LOCAL_INITIALIZER
//JOOS2:DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
//JAVAC:UNKNOWN

public class Je_8_DefiniteAssignment_UninitializedInNewArray {
	
	public Je_8_DefiniteAssignment_UninitializedInNewArray() {}

	public int method() {
		int[] a;
		a = new int[a[1]];
		return a[1];
	}

	public static int test() {
		return 123;
	}
}
