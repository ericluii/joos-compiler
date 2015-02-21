// JOOS1:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_CONSTRUCTOR_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Typecheck: 
 * - (Joos 1) Check that any method or constructor invocation resolves
 *   to a unique method with a type signature matching exactly the
 *   static types of the argument expressions.
 * - (Joos 2) Check that any method or constructor invocation resolves
 *   to a uniquely closest matching method or constructor (15.12.2).  
 */
public class Je_6_ConstructorPresent_IllegalConversion{

    public Je_6_ConstructorPresent_IllegalConversion(int a, short b) {}
    
    public Je_6_ConstructorPresent_IllegalConversion(short a, int b) {}
    
    public static int test() {
	new Je_6_ConstructorPresent_IllegalConversion(120, 3);
	return 123;
    }
}
