// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Returned expression type does not match method return type.
 */
public class Je_6_Assignable_ReturnInElse {
	
    public Je_6_Assignable_ReturnInElse(){}
    
    public String test(int foo) {
	
	if (foo == 1) return "1";
	else if (foo == 2) return "2";
	else {
	    return foo;
	}
	
    }

}
