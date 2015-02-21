// JOOS1:DISAMBIGUATION,TYPE_CHECKING,FIELD_NOT_FOUND
// JOOS2:DISAMBIGUATION,TYPE_CHECKING,FIELD_NOT_FOUND
// JAVAC:UNKNOWN
// 
/* Disambiguation:
 * Transform every AAmbiguousNameLvalue node into ...
 * ...
 * o A static field access (AStaticFieldLvalue) with a base type resolved from a
 *   prefix of the ambiguous name enclosed in zero or more non-static field
 *   accesses (ANonstaticFieldLvalue).
 * -----------------------------------------------------------------------------
 * The prefix 'javax' disambiguates into the type 'A.javax', so 'swing' is a
 * static field access and 'Action' and 'ACCELERATOR_KEY' are nonstatic field
 * accesses. 
 * 
 * That 'A.javax' does not have a static field 'swing' should not be checked
 * until TypeChecking, and most importantly it must not result in further 
 * disambiguation leading to 'javax.swing.Action.ACCELERATOR_KEY' being
 * disambiguated into a static field access of field 'ACCELERATOR_KEY' on the
 * type 'javax.swing.Action'.
 * 
 */

import A.*;

public class Main {
    public Main() {}
    
    public static int test() {
	String s = javax.swing.Action.ACCELERATOR_KEY;
	return 123;
    }
}
