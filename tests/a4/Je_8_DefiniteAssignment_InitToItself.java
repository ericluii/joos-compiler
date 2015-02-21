// JOOS1: DEFINITE_ASSIGNMENT,JOOS1_LOCAL_VARIABLE_IN_OWN_INITIALIZER
// JOOS2: DEFINITE_ASSIGNMENT,VARIABLE_MIGHT_NOT_HAVE_BEEN_INITIALIZED
// JAVAC: UNKNOWN
/**
 * Definite Assignment:
 * - (Joos 1) A local variable must not occur in its own initializer.
 * - (Joos 2) Whenever a local variable is used in any context which
 * is not the direct left-hand side of an assignment, it must be
 * definitely assigned at that point in the program. (a is not 
 * definitely assigned)
 */
public class Je_8_DefiniteAssignment_InitToItself {

    public Je_8_DefiniteAssignment_InitToItself() {}

    public static int test() {
	int a = a;
	return 123;
    }
}
