// TYPE_CHECKING,
// JOOS1: JOOS1_CLOSEST_MATCH_OVERLOADING,NO_MATCHING_CONSTRUCTOR_FOUND,AMBIGUOUS_OVERLOADING
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
public class Main implements Cloneable, Runnable{

    public Main(Cloneable s) {}
    
    public Main(Runnable s) {}

    public void run(){}

    public static int test() {
        new Main(new Main(new Thread()));
	return 123;
    }
}
