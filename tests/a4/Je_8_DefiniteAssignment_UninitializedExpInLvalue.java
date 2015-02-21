// JOOS1: DEFINITE_ASSIGNMENT,JOOS1_OMITTED_LOCAL_INITIALIZER
// JOOS2: DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
// JAVAC: UNKNOWN
/**
 * Definite Assignment:
 * - (Joos 1) A local variable must have an initalizer
 * - (Joos 2) A local variable must be initialized before it is used.
 */
public class Je_8_DefiniteAssignment_UninitializedExpInLvalue{
    
    public Je_8_DefiniteAssignment_UninitializedExpInLvalue(){}

    public static int test(){
	int[] z = new int[42];
	int x;   //Illegal joos1
	z[x] = x = 123;   //Illegal joos2
	return x;	
    }

}
