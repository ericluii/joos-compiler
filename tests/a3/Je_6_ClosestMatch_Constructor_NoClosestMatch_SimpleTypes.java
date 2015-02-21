// TYPE_CHECKING
// JOOS1: NO_MATCHING_CONSTRUCTOR_FOUND,AMBIGUOUS_OVERLOADING
// JOOS2: AMBIGUOUS_OVERLOADING
// JAVAC:UNKNOWN
/**
 * Typecheck: 
 * - (Joos 1) Check that any method or constructor invocation resolves
 *   to a unique method with a type signature matching exactly the
 *   static types of the argument expressions.
 * - (Joos 2) Check that any method or constructor invocation resolves
 *   to a uniquely closest matching method or constructor (15.12.2).  
 */
public class Je_6_ClosestMatch_Constructor_NoClosestMatch_SimpleTypes{

    public Je_6_ClosestMatch_Constructor_NoClosestMatch_SimpleTypes(int a, short b) {}
    
    public Je_6_ClosestMatch_Constructor_NoClosestMatch_SimpleTypes(short a, int b) {}
    
    public static int test() {
	new Je_6_ClosestMatch_Constructor_NoClosestMatch_SimpleTypes((short) 120, (short) 3);
	return 123;
    }
}
