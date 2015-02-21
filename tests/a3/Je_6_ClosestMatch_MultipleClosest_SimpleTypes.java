// TYPE_CHECKING,
// JOOS1:NO_MATCHING_METHOD_FOUND,AMBIGUOUS_OVERLOADING
// JOOS2:AMBIGUOUS_OVERLOADING
// JAVAC:UNKNOWN
/**
 * Typecheck: 
 * - (Joos 1) Check that any method or constructor invocation resolves
 *   to a unique method with a type signature matching exactly the
 *   static types of the argument expressions.
 * - (Joos 2) Check that any method or constructor invocation resolves
 *   to a uniquely closest matching method or constructor (15.12.2).  
 */
public class Je_6_ClosestMatch_MultipleClosest_SimpleTypes {

    public Je_6_ClosestMatch_MultipleClosest_SimpleTypes(){}

    public void a(short a, int b) {
	a((short)2, (short)2);
    }

    public void a(int a, short b) {
	
    }

    public static int test() { 
	return 123; 
    }
}
