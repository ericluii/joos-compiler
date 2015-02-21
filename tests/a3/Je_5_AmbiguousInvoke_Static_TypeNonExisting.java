// JOOS1:DISAMBIGUATION,VARIABLE_OR_TYPE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_OR_TYPE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/* Disambiguation:
 *   If none of the disambiguation rules apply, then an error message must be 
 *   produced.
 */
public class Je_5_AmbiguousInvoke_Static_TypeNonExisting {
    public Je_5_AmbiguousInvoke_Static_TypeNonExisting() {}
    
    public static int test() {
	java.lang.Integar.valueOf("123");
	return 123;
    }
}
