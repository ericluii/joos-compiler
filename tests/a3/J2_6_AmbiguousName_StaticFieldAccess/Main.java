// DISAMBIGUATION
// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
/* Disambiguation:
 * Transform every AAmbiguousNameLvalue node into ...
 * ...
 * o A static field access (AStaticFieldLvalue) with a base type resolved from a
 *   prefix of the ambiguous name enclosed in zero or more non-static field
 *   accesses (ANonstaticFieldLvalue).
 * -----------------------------------------------------------------------------
 * The class 'a.b.a' declares a static field 'c'.
 */
public class Main {
    public Main() {}
    
    public static int test() {
	return a.b.a.c;
    }
}
