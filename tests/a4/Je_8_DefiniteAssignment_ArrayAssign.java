// JOOS1: DEFINITE_ASSIGNMENT,JOOS1_OMITTED_LOCAL_INITIALIZER
// JOOS2: DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
// JAVAC: UNKNOWN
/**
 * Definite Assignment
 * - (Joos 1) A local variable declaration must have an initializer
 * - (Joos 2) A local variable must be initialized before it is used.
 *
 * This testcase tests whether an array access lvalue is treated as a
 * local variable in the analysis.  
 */
public class Je_8_DefiniteAssignment_ArrayAssign{

    public Je_8_DefiniteAssignment_ArrayAssign(){}

    public static int test(){
	int[] x;
	x[0] = 7;
	int [] y = x;
	return 123;
    }

}
