// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * - If the first identifier of the name disambiguates into a local
 * variable access, then a local variable with that name must be in
 * scope at that point in the program. Beware that a local variable is
 * not in scope before its declaration, even though it might be
 * present in the local variable environment of the current block.  
 */
public class Je_5_AmbiguousName_Local_UseBeforeDeclare {

    public Je_5_AmbiguousName_Local_UseBeforeDeclare(){}

    public static int test() {
	{
	    int a = b;
	    int b = 7;
	    return b;
	}
    }

}
