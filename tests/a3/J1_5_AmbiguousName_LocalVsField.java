// DISAMBIGUATION
/* Disambiguation:
 * 1) If the first identifier of the name disambiguates into a local variable
 *	access, then a local variable with that name must be in scope at that 
 *	point in the program. Beware that a local variable is not in scope
 *	before its declaration, even though it might be present in the local
 *	variable environment of the current block.
 * 2) If the first identifier of the name disambiguates into a non-static field
 * 	access, then a non-static field with that name must be in scope in the
 *	class containing the access.
 *
 */

public class J1_5_AmbiguousName_LocalVsField {
    public J1_5_AmbiguousName_LocalVsField() {}
    
    public int var = 123;
    
    public int method() {
	int b = var; // this is the field, since var is not declared yet
	int var = 42;
	return b;
    }
    
    public static int test() {
	J1_5_AmbiguousName_LocalVsField o = new J1_5_AmbiguousName_LocalVsField();
	return o.method();
    }
}
