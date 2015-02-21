// JOOS1:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
// JOOS2:TYPE_CHECKING,NO_MATCHING_METHOD_FOUND
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
public class Je_6_MethodPresent_IllegalConversion{

    public Je_6_MethodPresent_IllegalConversion() {}
    
    public void foo(int a, short b){}

    public void foo(short a, int b){}
    
    public static int test() {
	new Je_6_MethodPresent_IllegalConversion().foo(1, 2);
	return 123;
    }
}
